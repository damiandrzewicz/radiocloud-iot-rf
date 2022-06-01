
#include "RadioCloudNode.hpp"
#include <EEPROM.h>
#include "utils/LowPowerWrapper.hpp"
#include "message/concrete/RadioPairRequest.hpp"
#include "message/concrete/RadioPairResponse.hpp"

const char * const stateName[] PROGMEM = { 
    "ChekConfig",
    "RadioSend",
    "RadioReceive",
    "Sleep",
    "StateBtnDispatch",
    "RadioPair",
    "FactoryReset",
};


RadioCloudNode::RadioCloudNode(uint8_t stateBtnPin, uint8_t stateLedPin, int8_t extInterruptPin)
    : stateBtn_(stateBtnPin), stateLed_(stateLedPin), extInterruptPin_(extInterruptPin)
{

}
  
void RadioCloudNode::setup()
{
    stateBtn_.setDebounceTime(40);  // Set debounce time for buttons

    RadioCloudCore::setup();
}

void RadioCloudNode::loop()
{
    // StateBtn loop
    stateBtn_.loop();
    stateBtnPress_ = stateBtnCheck();

    // StateLed loop
    stateLed_.loop();

    RadioCloudCore::loop();
}

// Power saving operations
void RadioCloudNode::deepSleepFor(unsigned int delay)
{
    Log.verboseln(F("deep sleep for: %d ms"), delay);

    preDeepSleep();
    LowPowerWrapper::DeepSleep(delay);
    postDeepSleep();
}

void RadioCloudNode::deepSleepForWakeupOnInt(unsigned int delay, uint8_t pin, uint8_t mode)
{
    Log.verboseln(F("deep sleep for: %d, interrput(pin: %d, mode: %d)"), delay, pin, mode);

    preDeepSleep();
    attachInterrupt(digitalPinToInterrupt(pin), [](){}, mode);
    delay ? LowPowerWrapper::DeepSleep(delay) : LowPowerWrapper::DeepSleepForever();
    detachInterrupt(digitalPinToInterrupt(pin)); 
    postDeepSleep();
}

void RadioCloudNode::preDeepSleep()
{
    //Log.verboseln(F("Node::preDeepSleep"));

    Serial.flush();
    
    if(nodeContext_.radioInitialized)
    {
        radio_.sleep();
    }
}

void RadioCloudNode::postDeepSleep()
{
    //Log.verboseln(F("Node::postDeepSleep"));
} 

void RadioCloudNode::initStateMachine()
{
    // States
    stateMachine_.AddState(stateName[State::ChekConfig], 0, nullptr, [&](){ return onCheckConfig(); }, nullptr );
    stateMachine_.AddState(stateName[State::RadioSend], 0, nullptr, [&](){ return onRadioSend(); }, nullptr );
    stateMachine_.AddState(stateName[State::RadioReceive], 0, nullptr, [&](){ return onRadioReceive(); }, nullptr );
    stateMachine_.AddState(stateName[State::Sleep], 0, nullptr, [&](){ Log.verboseln("slp"); return onSleep(); }, nullptr );
    stateMachine_.AddState(stateName[State::StateBtnDispatch], 0, 
        [&](){ onEnterStateBtnDispatch(); }, 
        [&](){ return onStateBtnDispatch(); }, 
        [&](){ onLeaveStateBtnDispatch(); } 
    );
    stateMachine_.AddState(stateName[State::RadioPair], 10000, nullptr, [&](){ return onRadioPair(); }, nullptr );
    stateMachine_.AddState(stateName[State::FactoryReset], 0, nullptr, [&](){ return onFactoryReset(); }, nullptr );

    // Transitions
    stateMachine_.AddTransition(State::ChekConfig, State::Sleep, [&](auto *cs){
        return cs->result == FSM_StateResult::Error;
    });

    stateMachine_.AddTransition(State::ChekConfig, State::RadioSend, [&](auto *cs){
        return cs->result == FSM_StateResult::Ok;
    });

    stateMachine_.AddTransition(State::RadioSend, State::RadioReceive, [&](auto *cs){
        return cs->result == FSM_StateResult::Ok;
    });

    stateMachine_.AddTransition(State::RadioSend, State::StateBtnDispatch, [&](auto *cs){
        return cs->result == FSM_StateResult::Error;
    });

    stateMachine_.AddTransition(State::RadioReceive, State::StateBtnDispatch, [&](auto *cs){
        return cs->result != FSM_StateResult::Working;
    });

    stateMachine_.AddTransition(State::StateBtnDispatch, State::RadioPair, [&](auto *cs){
        return cs->timeout && this->stateBtnPress_ == StateBtnPress::Short;
    });

    stateMachine_.AddTransition(State::StateBtnDispatch, State::FactoryReset, [&](auto *cs){
        return cs->timeout && this->stateBtnPress_ == StateBtnPress::Long;
    });

    stateMachine_.AddTransition(State::StateBtnDispatch, State::Sleep, [&](auto *cs){
        return cs->timeout && this->stateBtn_.getState() == HIGH;
    });

    stateMachine_.AddTransition(State::RadioPair, State::ChekConfig, [&](auto *cs){
        return cs->result != FSM_StateResult::Working;
    });

    stateMachine_.AddTransition(State::FactoryReset, State::ChekConfig, [&](auto *cs){
        return cs->result != FSM_StateResult::Working;
    });

    stateMachine_.AddTransition(State::Sleep, State::ChekConfig, [&](auto *cs){
        return cs->result != FSM_StateResult::Working;
    });
}

RadioCloudNode::StateBtnPress RadioCloudNode::stateBtnCheck()
{
    //Log.verboseln("stateBtnCheck");
    static constexpr uint16_t shortPressTime = 3000;
    static constexpr uint16_t mediumPressTime = 10000;
    static constexpr uint16_t longPressTime = 15000;

    static unsigned long pressedTime = 0;
    static unsigned long releasedTime = 0;

    StateBtnPress press = StateBtnPress::NoPress;

    if(stateBtn_.isPressed()){
        pressedTime = millis();
        Log.verboseln(F("pressed: %d"), pressedTime);
    }
    
    if(stateBtn_.isReleased()) {
        releasedTime = millis();
        Log.verboseln(F("released: %d"), releasedTime);

        press = StateBtnPress::Single;
        long pressDuration = releasedTime - pressedTime;

        if( pressDuration >= shortPressTime ){
            press = StateBtnPress::Short;
        }

        if( (pressDuration >= mediumPressTime) && (pressDuration < longPressTime) ){
            press = StateBtnPress::Medium;
        }

        if( pressDuration >= longPressTime ){
            press = StateBtnPress::Long;
        }
    }
    //Log.verboseln(F("btnMode: %d, pressedTime: %d, releasedTime: %d"), 
    //   static_cast<uint8_t>(mode), pressedTime, releasedTime);

    //Log.verboseln(F("state: %d"), static_cast<int>(state));

    return press;
}

// Action handlers
FSM_StateResult RadioCloudNode::onCheckConfig()
{
    if(radioConfig_.isEmpty())
    {
        sleepTime_ = 0; // forever
        return FSM_StateResult::Error;
    }
    else
    {
        return FSM_StateResult::Ok;
    }
}

void RadioCloudNode::onEnterStateBtnDispatch()
{
    stateMachine_.CurrentState()->maxTime = stateBtn_.getDebounceTime() + 5;
    stateLed_.blink(50, 950, 1000);
}

FSM_StateResult RadioCloudNode::onStateBtnDispatch()
{
    //Log.verboseln(F("onStateBtnDispatch"));
    return FSM_StateResult::Working;
}

void RadioCloudNode::onLeaveStateBtnDispatch()
{
    //Log.verboseln(F("onLeaveStateBtnDispatch"));
    stateLed_.cancel();
}

FSM_StateResult RadioCloudNode::onRadioSend()
{
    return FSM_StateResult::Ok;
}

FSM_StateResult RadioCloudNode::onRadioReceive()
{
    return FSM_StateResult::Ok;
}

FSM_StateResult RadioCloudNode::onSleep()
{
    deepSleepForWakeupOnInt(sleepTime_, extInterruptPin_, FALLING);
    return FSM_StateResult::Ok;
}


FSM_StateResult RadioCloudNode::onRadioPair()
{
    auto currentState = stateMachine_.CurrentState();
    static auto success = false;
    auto res = FSM_StateResult::Working;
    static uint8_t step = 1;

    if(step == 1){
        stateLed_.blink(50, 250);
        radioSetup(2, radioConfigPairing_);
        step = 2;
    } else if(step == 2){
        if(!currentState->timeout){
            // Keep pairing
            static uint32_t execTime = 0;
            if(millis() - execTime > 1000L){
                execTime = millis();
                
                // Keep trying until send ok
                RadioPairRequest request(messageBuffer_);
                request.build();

                success = radioSend(radioConfigPairing_.gatewayId, true);
                
                if(success)
                {
                    // Parse message
                    RadioPairResponse response(messageBuffer_);
                    if(!response.parse())
                    {
                        Log.warningln(F("RadioPairResponse parse failed!"));
                        success = false;    // Back to previous step
                    }
                    else
                    {
                        Log.noticeln(F("got radio config: %d, %d, %lu, %s, %d"),
                            response.model().gatewayId,
                            response.model().networkId,
                            response.model().customFrequency,
                            response.model().encryptKey,
                            response.model().rssi
                        );

                        radioConfig_.gatewayId = response.model().gatewayId;
                        radioConfig_.networkId = response.model().networkId;
                        radioConfig_.customFrequency = response.model().customFrequency;
                        strcpy(radioConfig_.encryptKey, response.model().encryptKey);
                        radioConfig_.rssi = response.model().rssi;

                        EEPROM.put(0, radioConfig_);
                        
                        step = 3;   // Next step
                    }
                }
            }
        } else {
            step = 3;
        }
    } else if(step == 3){
        stateLed_.cancel();
        success ? stateLed_.blinkInPeriod(1000, 0, 3000) : stateLed_.blinkNumberOfTimes(500, 500, 3);
        step = 4;
    } else if(step == 4){
        if(stateLed_.getState() == LED_IDLE){
            res = success ? FSM_StateResult::Ok : FSM_StateResult::Error;
            step = 1;   
        }
    }

    return res;
}

FSM_StateResult RadioCloudNode::onFactoryReset()
{   
    return FSM_StateResult::Ok;
}