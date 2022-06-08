
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
    // StateLed loop
    stateLed_.loop();

    fsm_.loop();

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
    fsm_.addState(FSM_State::ChekConfig, stateName[FSM_State::ChekConfig], nullptr, [&](const auto &cs){ return onCheckConfig(); }, nullptr);
    fsm_.addState(FSM_State::RadioSend, stateName[FSM_State::RadioSend], nullptr, [&](const auto &cs){ return onRadioSend(); }, nullptr);
    fsm_.addState(FSM_State::RadioReceive, stateName[FSM_State::RadioReceive], nullptr, [&](const auto &cs){ return onRadioReceive(); }, nullptr);
    fsm_.addState(FSM_State::Sleep, stateName[FSM_State::Sleep], nullptr, [&](const auto &cs){ return onSleep(); }, nullptr);
    fsm_.addState(FSM_State::StateBtnDispatch, stateName[FSM_State::StateBtnDispatch], nullptr, [&](const auto &cs){ return onStateBtnDispatch(); }, nullptr);
    //fsm_.addState(FSM_State::RadioPair, stateName[FSM_State::RadioPair], nullptr, [&](const auto &cs){ return onCheckConfig(); }, nullptr);
    //fsm_.addState(FSM_State::FactoryReset, stateName[FSM_State::FactoryReset], nullptr, [&](const auto &cs){ return onCheckConfig(); }, nullptr);
}

// void RadioCloudNode::loopFSM()
// {
//     static auto transition = [&](FSM_State newState){
//         Log.traceln(F("fsm: %s->%s"), 
//             (const char *)pgm_read_word(&stateName[static_cast<int>(fsmState_)]), 
//             (const char *)pgm_read_word(&stateName[static_cast<int>(newState)])
//         );

//         fsmState_ = newState;
//     };
//     static auto enterState = [](FSM_State state){
//         Log.traceln(F(">>>%s"), 
//             (const char *)pgm_read_word(&stateName[static_cast<int>(state)])
//         );
//     };

//     static auto leaveState = [](FSM_State state){
//         Log.traceln(F("<<<%s"), 
//             (const char *)pgm_read_word(&stateName[static_cast<int>(state)])
//         );
//     };

//     auto currentState = fsmState_;
//     enterState(currentState);

//     if(fsmState_ == FSM_State::ChekConfig)
//     {
//         auto res = onCheckConfig();
//         if(isStateResultOk(res))
//         {
//             transition(FSM_State::RadioSend);
//         }
//         else if(isStateResultError(res))
//         {
//             transition(FSM_State::Sleep);
//         }
//     }
//     else if(fsmState_ == FSM_State::RadioSend)
//     {
//         auto res = onRadioSend();
//         if(isStateResultOk(res))
//         {
//             transition(FSM_State::RadioReceive);
//         }
//         else if(isStateResultError(res))
//         {
//             transition(FSM_State::RadioReceive);
//         }
//     }
//     else if(fsmState_ == FSM_State::RadioReceive)
//     {
//         auto res = onRadioReceive();
//         if(!isStateResultWorking(res))
//         {
//             transition(FSM_State::StateBtnDispatch);
//         }
//     }
//     else if(fsmState_ == FSM_State::Sleep)
//     {
//         auto res = onSleep();
//         if(!isStateResultWorking(res))
//         {
//             transition(FSM_State::ChekConfig);
//         }
//     }
//     else if(fsmState_ == FSM_State::StateBtnDispatch)
//     {
//         static uint8_t step = 1;

//         if(step == 1)
//         {
//             onEnterStateBtnDispatch();
//             ++step;
//         }
//         else if(step == 2)
//         {
//             const auto &state = stateBtn_.check();

//             if(state.step == ezButton::State::Step::Idle)
//             {
//                 transition(FSM_State::Sleep);
//             }
//             else if(state.step == ezButton::State::Step::Ready)
//             {
//                 auto pressTime = state.lastPressTime;
//                 Log.traceln(F("pressTime:%d"), pressTime);
//                 if(pressTime > StateBtnTimeout::mediumPressTime)
//                 {
//                     transition(FSM_State::FactoryReset);
//                 }
//                 else if(pressTime > StateBtnTimeout::shortPressTime)
//                 {
//                     transition(FSM_State::RadioPair);
//                 }
//             }

//             // Check if leave 
//             if(fsmState_ != FSM_State::StateBtnDispatch)
//             {
//                 onLeaveStateBtnDispatch();
//                 step = 1;
//             }
//         }
//     }
//     else if(fsmState_ == FSM_State::RadioPair)
//     {
//         auto res = onRadioPair();

//         static Delay::DelayNoBlockInfo info;
//         bool timeout = Delay::delayNoBlock(info, 10000);

//         if(timeout || !isStateResultWorking(res))
//         {
//             transition(FSM_State::ChekConfig);
//         }
//     }
//     else if(fsmState_ == FSM_State::FactoryReset)
//     {
//         auto res = onFactoryReset();
//         if(!isStateResultWorking(res))
//         {
//             transition(FSM_State::ChekConfig);
//         }
//     }
//     leaveState(currentState);
// }

// Action handlers
int8_t RadioCloudNode::onCheckConfig()
{
    if(radioConfig_.isEmpty())
    {
        sleepTime_ = 0; // forever
        return FSM_State::Sleep;
    }
    else
    {
        return FSM_State::RadioSend;
    }
}

void RadioCloudNode::onEnterStateBtnDispatch()
{
    //Log.noticeln(F("in"));
    stateLed_.blink(50, 950, 1000);
}

int8_t RadioCloudNode::onStateBtnDispatch()
{
    //Log.verboseln(F("onStateBtnDispatch"));

    return FSM_State::Sleep;
}

void RadioCloudNode::onLeaveStateBtnDispatch()
{
    //Log.noticeln(F("out"));
    stateLed_.cancel();
}

int8_t RadioCloudNode::onRadioSend()
{
    return FSM_State::RadioReceive;
}

int8_t RadioCloudNode::onRadioReceive()
{
    return FSM_State::StateBtnDispatch;
}

int8_t RadioCloudNode::onSleep()
{
    deepSleepForWakeupOnInt(sleepTime_, extInterruptPin_, FALLING);
    //Log.noticeln(F("fale slp"));
   // delay(2000);
    return FSM_State::ChekConfig;
}


int8_t RadioCloudNode::onRadioPair()
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

    return -1;
}

int8_t RadioCloudNode::onFactoryReset()
{   
    return -1;
}