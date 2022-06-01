
#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>
#include "core/RadioCloudCore.h"
#include "YA_FSM.h"
#include "NodeContext.hpp"
#include <ezButton.h>
#include <ezLED.h>

class RadioCloudNode : public RadioCloudCore
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

    enum State {
        ChekConfig,
        RadioSend,
        RadioReceive,
        Sleep,
        StateBtnDispatch,
        RadioPair,
        FactoryReset,
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
    virtual void initStateMachine() override;

    StateBtnPress stateBtnCheck();
    
    // Action handlers
    FSM_StateResult onCheckConfig();
    FSM_StateResult onRadioSend();
    FSM_StateResult onRadioReceive();
    FSM_StateResult onSleep();
    void onEnterStateBtnDispatch();
    FSM_StateResult onStateBtnDispatch();
    void onLeaveStateBtnDispatch();
    FSM_StateResult onRadioPair();
    FSM_StateResult onRadioReset();
    FSM_StateResult onFactoryReset();

private:
    NodeContext nodeContext_;
    ezButton stateBtn_;
    ezLED stateLed_;
    int8_t extInterruptPin_;
    MessageBuffer messageBuffer_;

    StateBtnPress stateBtnPress_ = StateBtnPress::NoPress;
    unsigned long sleepTime_ = 0;
};