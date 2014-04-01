
#pragma once


class FSM {
public:
    ~FSM();
    static FSM* GetInstance();


private:
    FSM();

    enum {
        ST_WAIT = 0,
        ST_GAME,
    }stat_;
};

