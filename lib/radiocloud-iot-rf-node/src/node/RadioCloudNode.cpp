
#include "RadioCloudNode.hpp"
#include <EEPROM.h>
#include "utils/LowPowerWrapper.hpp"
#include "utils/Delay.hpp"
#include "message/concrete/RadioPairRequest.hpp"
#include "message/concrete/RadioPairResponse.hpp"

const char * const stateName[] PROGMEM = { 
    "ChekConfig",
    "RadioSend",
    "RadioReceive",
    "Sleep",
    "StateBtnDispatch",
    "RadioPair",
    "FactoryReset"
};


RadioCloudNode::RadioCloudNode(uint8_t stateBtnPin, uint8_t stateLedPin, int8_t extInterruptPin)
    : stateBtn_(stateBtnPin), stateLed_(stateLedPin), extInterruptPin_(extInterruptPin)
{

}
  
void RadioCloudNode::setup()
{
    stateBtn_.setDebounceTime(40);  // Set debounce time for buttons

    initFSM();
    RadioCloudCore::setup();
}

void RadioCloudNode::loop()
{
    // StateBtn loop
    //stateBtn_.loop();
    //stateBtnPress_ = stateBtnCheck();
    
    // StateLed loop
    stateLed_.loop();

    loopFSM();

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
    Log.verboseln(F("Node::postDeepSleep"));
} 

void RadioCloudNode::initFSM()
{
    fsmState_ = FSM_State::ChekConfig;
}

void RadioCloudNode::loopFSM()
{
    static auto transition = [&](FSM_State newState){
        Log.traceln(F("fsm: %s->%s"), 
            (const char *)pgm_read_word(&stateName[static_cast<int>(fsmState_)]), 
            (const char *)pgm_read_word(&stateName[static_cast<int>(newState)])
        );

        fsmState_ = newState;
    };
    static auto enterState = [](FSM_State state){
        Log.traceln(F(">>>%s"), 
            (const char *)pgm_read_word(&stateName[static_cast<int>(state)])
        );
    };

    static auto leaveState = [](FSM_State state){
        Log.traceln(F("<<<%s"), 
            (const char *)pgm_read_word(&stateName[static_cast<int>(state)])
        );
    };


    if(fsmState_ == FSM_State::ChekConfig)
    {
        enterState(FSM_State::ChekConfig);
        auto res = onCheckConfig();
        if(isStateResultOk(res))
        {
            transition(FSM_State::RadioSend);
        }
        else if(isStateResultError(res))
        {
            transition(FSM_State::Sleep);
        }
        leaveState(FSM_State::ChekConfig);
    }
    else if(fsmState_ == FSM_State::RadioSend)
    {
        enterState(FSM_State::RadioSend);
        auto res = onRadioSend();
        if(isStateResultOk(res))
        {
            transition(FSM_State::RadioReceive);
        }
        else if(isStateResultError(res))
        {
            transition(FSM_State::RadioReceive);
        }
        leaveState(FSM_State::RadioSend);
    }
    else if(fsmState_ == FSM_State::RadioReceive)
    {
        enterState(FSM_State::RadioReceive);
        auto res = onRadioReceive();
        if(!isStateResultWorking(res))
        {
            transition(FSM_State::StateBtnDispatch);
        }
        leaveState(FSM_State::RadioReceive);
    }
    else if(fsmState_ == FSM_State::Sleep)
    {
        enterState(FSM_State::Sleep);
        
        auto res = onSleep();
        if(!isStateResultWorking(res))
        {
            transition(FSM_State::ChekConfig);
        }
    

        leaveState(FSM_State::Sleep);
    }
    else if(fsmState_ == FSM_State::StateBtnDispatch)
    {
        enterState(FSM_State::StateBtnDispatch);
        static uint8_t step = 1;

        if(step == 1)
        {
            onEnterStateBtnDispatch();
            ++step;
        }
        else if(step == 2)
        {
            //onStateBtnDispatch();
            stateBtn_.check();
            const auto &state = stateBtn_.getState();

            if(state.step == ezButton::State::Step::Idle)
            {
                transition(FSM_State::Sleep);
            }
            else if(state.step == ezButton::State::Step::Ready)
            {
                auto pressTime = state.lastPressTime;
                Log.traceln(F("pressTime:%d"), pressTime);
                if(pressTime > 6000)
                {
                    transition(FSM_State::FactoryReset);
                }
                else if(pressTime > 3000)
                {
                    transition(FSM_State::RadioPair);
                }
            }

            // Check if leave 
            if(fsmState_ != FSM_State::StateBtnDispatch)
            {
                onLeaveStateBtnDispatch();
                step = 1;
            }
        }


        leaveState(FSM_State::StateBtnDispatch);
    }
    else if(fsmState_ == FSM_State::RadioPair)
    {
        enterState(FSM_State::RadioPair);
        auto res = onRadioPair();

        static Delay::DelayNoBlockInfo info;
        bool timeout = Delay::delayNoBlock(info, 10000);

        if(timeout || !isStateResultWorking(res))
        {
            transition(FSM_State::ChekConfig);
        }
        leaveState(FSM_State::RadioPair);
    }
    else if(fsmState_ == FSM_State::FactoryReset)
    {
        enterState(FSM_State::FactoryReset);
        auto res = onFactoryReset();
        if(!isStateResultWorking(res))
        {
            transition(FSM_State::ChekConfig);
        }
        leaveState(FSM_State::FactoryReset);
    }
}

RadioCloudNode::StateBtnPress RadioCloudNode::stateBtnCheck()
{
    static constexpr uint16_t shortPressTime = 3000;
    static constexpr uint16_t mediumPressTime = 10000;
    static constexpr uint16_t longPressTime = 15000;

    static unsigned long pressedTime = 0;
    static unsigned long releasedTime = 0;

    StateBtnPress press = StateBtnPress::NoPress;

    // if(stateBtn_.isPressed()){
    //     pressedTime = millis();
    //     Log.verboseln(F("pressed: %d"), pressedTime);
    // }
    
    // if(stateBtn_.isReleased()) {
    //     releasedTime = millis();
    //     Log.verboseln(F("released: %d"), releasedTime);

    //     press = StateBtnPress::Single;
    //     long pressDuration = releasedTime - pressedTime;

    //     if( pressDuration >= shortPressTime ){
    //         press = StateBtnPress::Short;
    //     }

    //     if( (pressDuration >= mediumPressTime) && (pressDuration < longPressTime) ){
    //         press = StateBtnPress::Medium;
    //     }

    //     if( pressDuration >= longPressTime ){
    //         press = StateBtnPress::Long;
    //     }
    // }
    //Log.verboseln(F("btnMode: %d, pressedTime: %d, releasedTime: %d"), 
    //   static_cast<uint8_t>(mode), pressedTime, releasedTime);

    //Log.verboseln(F("state: %d"), static_cast<int>(state));

    return press;
}

// Action handlers
FSM::StateResult RadioCloudNode::onCheckConfig()
{
    if(radioConfig_.isEmpty())
    {
        sleepTime_ = 0; // forever
        return FSM::StateResult::Error;
    }
    else
    {
        return FSM::StateResult::Ok;
    }
}

void RadioCloudNode::onEnterStateBtnDispatch()
{
    //Log.noticeln(F("in"));
    stateLed_.blink(50, 950, 1000);
}

FSM::StateResult RadioCloudNode::onStateBtnDispatch()
{
    //Log.verboseln(F("onStateBtnDispatch"));

    return FSM::StateResult::Working;
}

void RadioCloudNode::onLeaveStateBtnDispatch()
{
    //Log.noticeln(F("out"));
    stateLed_.cancel();
}

FSM::StateResult RadioCloudNode::onRadioSend()
{
    return FSM::StateResult::Ok;
}

FSM::StateResult RadioCloudNode::onRadioReceive()
{
    return FSM::StateResult::Ok;
}

FSM::StateResult RadioCloudNode::onSleep()
{
    deepSleepForWakeupOnInt(sleepTime_, extInterruptPin_, FALLING);
    //Log.noticeln(F("fale slp"));
   // delay(2000);
    return FSM::StateResult::Ok;
}


FSM::StateResult RadioCloudNode::onRadioPair()
{
    // auto currentState = stateMachine_.CurrentState();
    // static auto success = false;
    // auto res = FSM_StateResult::Working;
    // static uint8_t step = 1;

    // if(step == 1){
    //     stateLed_.blink(50, 250);
    //     radioSetup(2, radioConfigPairing_);
    //     step = 2;
    // } else if(step == 2){
    //     if(!currentState->timeout){
    //         // Keep pairing
    //         static uint32_t execTime = 0;
    //         if(millis() - execTime > 1000L){
    //             execTime = millis();
                
    //             // Keep trying until send ok
    //             RadioPairRequest request(messageBuffer_);
    //             request.build();

    //             success = radioSend(radioConfigPairing_.gatewayId, true);
                
    //             if(success)
    //             {
    //                 // Parse message
    //                 RadioPairResponse response(messageBuffer_);
    //                 if(!response.parse())
    //                 {
    //                     Log.warningln(F("RadioPairResponse parse failed!"));
    //                     success = false;    // Back to previous step
    //                 }
    //                 else
    //                 {
    //                     Log.noticeln(F("got radio config: %d, %d, %lu, %s, %d"),
    //                         response.model().gatewayId,
    //                         response.model().networkId,
    //                         response.model().customFrequency,
    //                         response.model().encryptKey,
    //                         response.model().rssi
    //                     );

    //                     radioConfig_.gatewayId = response.model().gatewayId;
    //                     radioConfig_.networkId = response.model().networkId;
    //                     radioConfig_.customFrequency = response.model().customFrequency;
    //                     strcpy(radioConfig_.encryptKey, response.model().encryptKey);
    //                     radioConfig_.rssi = response.model().rssi;

    //                     EEPROM.put(0, radioConfig_);
                        
    //                     step = 3;   // Next step
    //                 }
    //             }
    //         }
    //     } else {
    //         step = 3;
    //     }
    // } else if(step == 3){
    //     stateLed_.cancel();
    //     success ? stateLed_.blinkInPeriod(1000, 0, 3000) : stateLed_.blinkNumberOfTimes(500, 500, 3);
    //     step = 4;
    // } else if(step == 4){
    //     if(stateLed_.getState() == LED_IDLE){
    //         res = success ? FSM_StateResult::Ok : FSM_StateResult::Error;
    //         step = 1;   
    //     }
    // }

    // return res;
}

FSM::StateResult RadioCloudNode::onFactoryReset()
{   
    return FSM::StateResult::Ok;
}