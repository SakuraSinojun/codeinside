

#pragma once

#include "window/Rect.h"
#include "tools/socket.h"
#include "packet/game_start_packet.h"
#include <string>

namespace game_info {

const int SEAT_SELF = 0;
const int SEAT_DOWNRIGHT = 1;
const int SEAT_UPRIGHT = 2;
const int SEAT_UP = 3;
const int SEAT_UPLEFT = 4;
const int SEAT_UPDOWN = 5;


void SetPlayerName(std::string  name);
std::string GetPlayerName(void);

void SetMainSocket(tools::CSocket& sock);
tools::CSocket&  GetMainSocket(void);

void SetGameStartPacket(GameStartPacket& packet);
GameStartPacket* GetGameStartPacket(void);
int PlayerNameToSeat(std::string playerid);
std::string SeatToPlayerName(int seat);


bool IsMate(int seat1, int seat2);
bool IsMate(std::string player1, std::string player2);
bool IsOpposite(int seat1, int seat2);
bool IsOpposite(std::string player1, std::string player2);

CSize GetRenderSize(void);
CRect GetHandCardsRect(void);
CSize GetCardSize(void);

CRect GetPlayerRect(int seat);
CRect GetClockRect(int seat);

CRect GetPlayButtonRect(void);
CRect GetPassButtonRect(void);
CRect GetShaoButtonRect(void);

} // namespace game_info

