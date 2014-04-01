

#pragma once

#include <vector>
#include <string>

class Player;
class Packet;
class Poker;
class Game {
public:
    ~Game();
    static Game* GetInstance();

    bool CanAcceptPlayers(void) { return CountPlayers() < MaxPlayers(); }
    void JoinPlayer(Player* player);
    void LeavePlayer(Player* player);
    int  CountPlayers(void) { return player_vector_.size(); }
    int  MaxPlayers(void) { return 6; }

    std::string GeneratePlayerName(std::string name);

    void OnPlay(Player* player, std::string cards);

public:
    void BroadCast(Packet* packet);
    void BroadcastServerInfo(void);
    void StartGame(void);
    void ShuffleAndDeal(void);
    void SpreadPlayerCards(void);
    void BroadcastScene(void);

private:
    Game();

    int GetPlayerId(Player* player);
    Player* GetPlayer(int seat);

    std::vector<Player*>    player_vector_;
    Poker*  poker_;
};


