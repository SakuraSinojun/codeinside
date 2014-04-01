
#include "game.h"
#include "game_window.h"
#include "info.h"
#include "window/Rect.h"
#include <windows.h>


void GameApp::Init()
{
    CRect   rect(CW_USEDEFAULT, CW_USEDEFAULT, 960, 600);
    m_window = new GameWindow();
    m_window->Create(NULL, rect, TEXT("Game"));
    m_window->Show();
}

void game_start(tools::CSocket& sock, GameStartPacket& packet)
{
    game_info::SetMainSocket(sock);
    game_info::SetGameStartPacket(packet);

    GameApp app;
    app.loop();
}


