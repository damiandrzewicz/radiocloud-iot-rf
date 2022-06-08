#pragma once

#include <ArduinoLog.h>
#include "nonstd.h"


template <size_t N>
class FSM
{
public:

    struct StateInfo
    {
        bool isEnterTimeout = false;
        bool isStateExecTimeout = false;
    };

    typedef nonstd::function<void()> ProxyCbk;
    typedef nonstd::function<int8_t(const StateInfo &)> StateCbk;

    struct State
    {
        const char *name;
        ProxyCbk onEnterCbk = nullptr;
        StateCbk onStateCbk = nullptr;
        ProxyCbk onLeaveCbk = nullptr;
        unsigned long enterDelay = 0;
        unsigned long stateTimeout = 0;
        StateInfo info;
    };

    virtual ~FSM(){}

    bool addState(uint8_t stateIndex, const char *name, ProxyCbk onEnterCbk, StateCbk onStateCbk, ProxyCbk onLeaveCbk,
        unsigned long stateTimeout = 0, unsigned long enterDelay = 0)
    {
        Log.traceln(F("stateIndex: %d, size: %d"), stateIndex, N);
        if(stateIndex > N)
        {
            return false;
        }

        auto &state = states_[stateIndex];
        state.name = name;

        Log.traceln(F("before nptr: %d"), state.onEnterCbk != nullptr);
        state.onEnterCbk = onEnterCbk;
        Log.traceln(F("after nptr: %d"), state.onEnterCbk != nullptr);

        state.onStateCbk = onStateCbk;
        state.onLeaveCbk = onLeaveCbk;
        state.enterDelay = enterDelay;
        state.stateTimeout = stateTimeout;

        return true;
    }

    StateInfo loop()
    {
        Log.traceln(F("loop idx: %d"), currentStateIndex_);
        State &state = states_[currentStateIndex_];

        auto currentTime = millis();
        StateInfo stateInfo;

        Log.traceln(F("nptr: %d"), state.onEnterCbk != nullptr);
        
        if(state.onEnterCbk)
        {
            if(!enterStartTime_)
            {
                state.info.isEnterTimeout = false;
                enterStartTime_ = currentTime;
                Log.traceln(F("fsm[%s]->enter: delay init"), state.name);
            }

            if(currentTime - enterStartTime_ < state.enterDelay)
            {
                return stateInfo;
            }
            
            enterStartTime_ = 0;
            state.info.isEnterTimeout = true;

            Log.traceln(F("fsm[%s]->enter: exec"), state.name);
            state.onEnterCbk();
        }
        
        if(!stateStartTime_)
        {
            state.info.isStateExecTimeout = false;
            stateStartTime_ = currentTime;
            Log.traceln(F("fsm[%s]->state: timeout init"), state.name);
        }
        if(currentTime - stateStartTime_ < state.stateTimeout)
        {
            return stateInfo;
        }
        
        stateStartTime_ = 0;
        state.info.isStateExecTimeout = true;

        Log.traceln(F("fsm[%s]->state: exec"), state.name);
        uint8_t nextStateIndex = state.onStateCbk(state.info);

        if(state.onLeaveCbk)
        {
            Log.traceln(F("fsm[%s]->leave: exec"), state.name);
            state.onLeaveCbk();
        }
        
        if(nextStateIndex != -1 && currentStateIndex_ != nextStateIndex)
        {
            Log.traceln(F("fsm[%s]->transition to [%s]"), state.name, states_[nextStateIndex].name);
            currentStateIndex_ = nextStateIndex;
        }

        return stateInfo;
    }

private:
    State states_[N ? N : 1];
    uint8_t currentStateIndex_ = 0;

    unsigned long enterStartTime_ = 0;
    unsigned long stateStartTime_ = 0;

    uint8_t step = 1;
};