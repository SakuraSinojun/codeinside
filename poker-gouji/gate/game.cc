

#include "game.h"
#include "player.h"
#include "poker.h"

#include "packet/server_info_packet.h"
#include "packet/game_start_packet.h"
#include "packet/deal_packet.h"
#include "packet/scene_packet.h"
#include "packet/play_packet.h"

#include "rules/gouji.h"

#include <algorithm>
#include <sstream>
#include <time.h>
#include "log.h"
// DECLARE_MODULE(game);

static Game* g_game = NULL;

Game::Game()
    : poker_(new Poker())
{
}

Game::~Game()
{
    if (poker_)
        delete poker_;
}

Game* Game::GetInstance()
{
    if (g_game == NULL)
        g_game = new Game();
    return g_game;
}

void Game::JoinPlayer(Player* player)
{
    std::vector<Player*>::iterator iter;
    iter = std::find(player_vector_.begin(), player_vector_.end(), player);
    if (iter == player_vector_.end()) {
        player_vector_.push_back(player);
        BroadcastServerInfo();
        if (CountPlayers() == MaxPlayers()) {
            StartGame();
        }
    }
}

void Game::LeavePlayer(Player* player)
{
    std::vector<Player*>::iterator iter;
    iter = std::find(player_vector_.begin(),
        player_vector_.end(),
        player);
    if (iter != player_vector_.end())
        player_vector_.erase(iter);
    BroadcastServerInfo();
}

void Game::BroadCast(Packet* packet)
{
    std::vector<Player*>::iterator  iter;
    for (iter = player_vector_.begin();
        iter != player_vector_.end();
        iter++) {
        Player* p = *iter;
        p->SendPacket(packet);
    }
}

void Game::BroadcastServerInfo(void)
{
    std::stringstream   oss;
    std::vector<Player*>::iterator  iter;
    for (iter = player_vector_.begin();
        iter != player_vector_.end();
        iter++) {
        Player* p = *iter;
        oss << p->GetRemoteIP() << '(' << p->GetRemotePort() << ')';
        oss << "\t\t";
        oss << p->id();
        oss << '\n';
    }

    ServerInfoPacket    packet;
    packet.Players() = CountPlayers();
    packet.MaxPlayers() = MaxPlayers();
    packet.ServerInfo() = "Poker Server";
    packet.Message() = "players:\n" + oss.str();

    BroadCast(&packet);
}

void Game::StartGame()
{
    GameStartPacket packet;
    packet.Clear();
    int index = 0;
    std::vector<Player*>::iterator  iter;
    for (iter = player_vector_.begin();
        iter != player_vector_.end();
        iter++) {
        Player* p = *iter;
        p->seatid() = index;
        index ++;
        packet.AddSeat(p->id());
    }
    BroadCast(&packet);
    ShuffleAndDeal();
}

void Game::SpreadPlayerCards(void)
{
    std::vector<Player*>::iterator  iter;
    for (iter = player_vector_.begin();
        iter != player_vector_.end();
        iter++) {
        Player* p = *iter;
        DealPacket  packet;
        packet.Cards() = rules::SortCards(poker_->GetPlayerCards(p->seatid()));
        p->SendPacket(&packet);
    }
}

void Game::ShuffleAndDeal(void)
{
    poker_->Shuffle();
    poker_->Deal();

    SpreadPlayerCards();
    BroadcastScene();
}

void Game::BroadcastScene(void)
{
    ScenePacket packet;
    Poker::Scene    scene = poker_->NextTurn();
 
    packet.Cards() = rules::SortCards(scene.cards);
    
    if (scene.cards_owner >= 0 && scene.cards_owner < (int)player_vector_.size())
        packet.Owner() = player_vector_[scene.cards_owner]->id();
    else
        packet.Owner() = std::string("");

    if (scene.reasonable_player >= 0 && scene.reasonable_player < (int)player_vector_.size()) {
        packet.Player() = player_vector_[scene.reasonable_player]->id();
    } else {
        packet.Player() = player_vector_[0]->id();
        // throw ("Cannot be here.");
    }
    
    std::vector<Player*>::iterator  iter;
    for (iter = player_vector_.begin(); iter != player_vector_.end(); iter++) {
        Player* p = *iter;
        packet.SetPlayerStates(p->id(), "NORMAL");
        if (poker_->player_states_[p->seatid()].is_failure_)
            packet.SetPlayerStates(p->id(), std::string("FAILURE"));
        if (poker_->GetPlayerCards(p->seatid()).empty())
            packet.SetPlayerStates(p->id(), std::string("SUCCESS"));
    }
    BroadCast(&packet);
}

std::string Game::GeneratePlayerName(std::string name)
{
    ScopeTrace(x);
    std::string newname = name;

    while (true) {
        std::vector<Player*>::iterator  iter;
        for (iter = player_vector_.begin();
            iter != player_vector_.end();
            iter++) {
            Player* p = *iter;
            if (p->id() == newname) {
                std::stringstream   oss;
                oss << (time(NULL) % 10000);
                newname = name + std::string("_") + oss.str();
                break;
            }
        }
        if (iter == player_vector_.end())
            break;
    }
    return newname;
}

int Game::GetPlayerId(Player* player)
{
    if (!player)
        return -1;

    size_t i;
    for (i=0; i<player_vector_.size(); i++) {
        Player* p = player_vector_[i];
        if (p == player)
            return i;
    }
    return -1;
}

Player* Game::GetPlayer(int seat)
{
    if (seat >= 0 && seat < (int)player_vector_.size())
        return player_vector_[seat];
    return NULL;
}

void Game::OnPlay(Player* player, std::string cards)
{
    if (!player)
        return;

    PlayPacket  packet;
    packet.Ensure() = false;

    if (poker_->last_scene_.reasonable_player != player->seatid()) {
        player->SendPacket(&packet);
        return;
    }

    if (!poker_->Play(cards)) {
        player->SendPacket(&packet);
        return;
    }
    packet.Ensure() = true;
    
    player->SendPacket(&packet);

    DealPacket  dpacket;
    dpacket.Cards() = rules::SortCards(poker_->GetPlayerCards(player->seatid()));
    player->SendPacket(&dpacket);

    BroadcastScene();

}

