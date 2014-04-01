
#pragma once

#include "tools/socket.h"
#include "window/App.h"
#include "packet/game_start_packet.h"
#include "game_window.h"

class GameApp : public CApp {
public:
    void Init();
private:
    GameWindow * m_window;
};

void game_start(tools::CSocket& sock, GameStartPacket& packet);

