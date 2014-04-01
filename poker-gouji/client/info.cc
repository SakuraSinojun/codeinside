

#include "info.h"

#include <string>

namespace game_info {

static std::string          g_player_name("Player");
static tools::CSocket       g_socket;
static GameStartPacket *    g_gamestart_packet = NULL;

void SetPlayerName(std::string  name)
{
    g_player_name = name;
}

std::string GetPlayerName(void)
{
    return g_player_name;
}

void SetMainSocket(tools::CSocket& sock)
{
    g_socket = sock;
}

tools::CSocket&  GetMainSocket(void)
{
    return g_socket;
}

void SetGameStartPacket(GameStartPacket& packet)
{
    g_gamestart_packet = &packet;
}

GameStartPacket* GetGameStartPacket(void)
{
    return g_gamestart_packet;
}

int PlayerNameToSeat(std::string playerid)
{
    int self = -1;
    int index = -1;
    int i;
    for (i=0; i<g_gamestart_packet->Seats(); i++) {
        std::string name = g_gamestart_packet->At(i);
        if (name == g_player_name) {
            self = i;
        }
        if (name == playerid)
            index = i;
    }
    if (self == -1 || index == -1)
        return -1;
    return (6 + index - self) % 6;
}

std::string SeatToPlayerName(int seat)
{
    if (seat >= 0 && seat < g_gamestart_packet->Seats())
        return g_gamestart_packet->At(seat);
    return std::string("");
}


bool IsMate(int seat1, int seat2)
{
    if (seat1 < 0 || seat1 >= 6 || seat2 < 0 || seat2 >= 0)
        return false;

    if ((seat1 % 2) == (seat2 % 2))
        return true;
    return false;
}

bool IsMate(std::string player1, std::string player2)
{
    int  seat1 = PlayerNameToSeat(player1);
    int  seat2 = PlayerNameToSeat(player2);
    return IsMate(seat1, seat2);
}

bool IsOpposite(int seat1, int seat2)
{
    if (seat1 < 0 || seat1 >= 6 || seat2 < 0 || seat2 >= 0)
        return false;

    if ( ((6 + seat1 - seat2) % 6) == 3)
        return true;
    return false;
}

bool IsOpposite(std::string player1, std::string player2)
{
    int  seat1 = PlayerNameToSeat(player1);
    int  seat2 = PlayerNameToSeat(player2);
    return IsOpposite(seat1, seat2);
}


/////////////////////////////////////////////////////////////////////////////////////

CSize GetRenderSize(void)
{
    return CSize(1600, 1000);
}

CRect GetHandCardsRect(void)
{
    return CRect(100, 800, 1300, 200);
}

CSize GetCardSize(void)
{
    return CSize(125, 175);
}

CRect GetPlayButtonRect(void)
{
    return CRect(1450, 800, 50, 50);
}

CRect GetPassButtonRect(void)
{
    return CRect(1450, 860, 50, 50);
}

CRect GetShaoButtonRect(void)
{
    return CRect(1450, 920, 50, 50);
}

CRect GetPlayerRect(int seat)
{
    CSize size = GetRenderSize();
    int width = 450;
    int height = 200;
    int left = 50;

    CRect   rcs[] = {
        CRect((size.cx - width) / 2,  550, width, height),
        CRect(size.cx - width - left, 430, width, height),
        CRect(size.cx - width - left, 220, width, height),
        CRect((size.cx - width) / 2,   10, width, height),
        CRect(left,                   220, width, height),
        CRect(left,                   430, width, height)
    };

    if (seat >=0 && seat < 6)
        return rcs[seat];
    return CRect(0, 0, 0, 0);
}

CRect GetClockRect(int seat)
{
    return GetPlayerRect(seat);
}

} // namespace game_info


