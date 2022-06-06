
#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>
#include "core/RadioCloudCore.h"
#include "NodeContext.hpp"
#include <ezButton.h>
#include <ezLED.h>
#include "core/FSM.hpp"

class RadioCloudNode : public RadioCloudCore, public FSM
{
public:

    enum class StateBtnPress
    {
        NoPress,
        Single,
        Short,
        Medium,
        Long
    };

    enum FSM_State {
        ChekConfig,
        RadioSend,
        RadioReceive,
        Sleep,
        StateBtnDispatch,
        RadioPair,
        FactoryReset
    };

    RadioCloudNode(uint8_t stateBtnPin = 4, uint8_t stateLedPin = 9, int8_t extInterruptPin = 3);
  
    virtual void setup() override;
    virtual void loop();

    // Power saving operations
    void deepSleepFor(unsigned int delay);
    void deepSleepForWakeupOnInt(unsigned int delay, uint8_t pin, uint8_t mode);
    void preDeepSleep();
    void postDeepSleep();

protected:
    virtual void initFSM() override;
    virtual void loopFSM() override;

    StateBtnPress stateBtnCheck();
    
    // Action handlers
    FSM::StateResult onCheckConfig();
    FSM::StateResult onRadioSend();
    FSM::StateResult onRadioReceive();
    FSM::StateResult onSleep();
    void onEnterStateBtnDispatch();
    FSM::StateResult onStateBtnDispatch();
    void onLeaveStateBtnDispatch();
    FSM::StateResult onRadioPair();
    FSM::StateResult onRadioReset();
    FSM::StateResult onFactoryReset();

private:
    NodeContext nodeContext_;
    ezButton stateBtn_;
    ezLED stateLed_;
    int8_t extInterruptPin_;
    MessageBuffer messageBuffer_;

    StateBtnPress stateBtnPress_ = StateBtnPress::NoPress;
    unsigned long sleepTime_ = 0;

    FSM_State fsmState_;
};