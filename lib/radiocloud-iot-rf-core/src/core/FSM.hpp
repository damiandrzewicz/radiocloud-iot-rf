#pragma once

class FSM
{
public:

protected:

    virtual ~FSM(){}

    enum class StateResult
    {
        Ok,
        Error,
        Working
    };

    virtual void initFSM() = 0;
    virtual void loopFSM() = 0;

    bool isStateResultOk(StateResult state){ return state == StateResult::Ok; }
    bool isStateResultError(StateResult state){ return state == StateResult::Error; }
    bool isStateResultWorking(StateResult state){ return state == StateResult::Working; }

private:
};