
#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>
#include "core/RadioCloudCore.h"
#include "NodeContext.hpp"
#include <ezButton.h>
#include <ezLED.h>
#include "core/FSM.hpp"

class RadioCloudNode : public RadioCloudCore
{
public:

    struct StateBtnTimeout
    {
        static constexpr uint16_t shortPressTime = 3000;
        static constexpr uint16_t mediumPressTime = 10000;
        static constexpr uint16_t longPressTime = 15000;
    };

    enum FSM_State {
        ChekConfig,
        RadioSend,
        RadioReceive,
        Sleep,
        StateBtnDispatch,
        RadioPair,
        FactoryReset,
        _Size_
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
    virtual void initFSM();
    
    // Action handlers
    int8_t onCheckConfig();
    int8_t onRadioSend();
    int8_t onRadioReceive();
    int8_t onSleep();
    void onEnterStateBtnDispatch();
    int8_t onStateBtnDispatch();
    void onLeaveStateBtnDispatch();
    int8_t onRadioPair();
    int8_t onRadioReset();
    int8_t onFactoryReset();

private:
    NodeContext nodeContext_;
    ezButton stateBtn_;
    ezLED stateLed_;
    int8_t extInterruptPin_;
    MessageBuffer messageBuffer_;

    unsigned long sleepTime_ = 0;

    FSM<FSM_State::_Size_> fsm_;
};