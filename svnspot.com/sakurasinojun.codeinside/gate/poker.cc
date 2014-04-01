

#include "poker.h"
#include "util/string.h"
#include "rules/gouji.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>
#include <cassert>
#include <time.h>


Poker::Poker()
{
    AddCardSet();
    AddCardSet();
    AddCardSet();
    AddCardSet();
}

Poker::~Poker()
{
}

void Poker::Shuffle(void)
{
    int i, j;
    int times;
    std::string temp;
    srand((unsigned int)time(NULL));

    for (times=0; times<100000; times++) {
        i = rand() % cards_vector_.size();
        j = rand() % cards_vector_.size();
        temp = cards_vector_[i];
        cards_vector_[i] = cards_vector_[j];
        cards_vector_[j] = temp;
    }
}

void Poker::Deal(int players)
{
    player_cards_.clear();
    int i = 0;
    for (i=0; i<players; i++) {
        player_cards_.push_back(std::string(""));
        player_states_.push_back(PLAYERSTATES());
    }

    i = 0;
    std::vector<std::string>::iterator  iter;
    for (iter=cards_vector_.begin(); iter!=cards_vector_.end(); iter++) {
        std::string cards = player_cards_[i];
        cards += *iter;
        player_cards_[i] = cards;
        i++;
        if (i>=players)
            i = 0;
    }
}

std::string Poker::GetPlayerCards(int player)
{
    if (player >= 0 && player <= (int)player_cards_.size())
        return player_cards_[player];
    else
        return std::string("");
}

void Poker::AddCard(std::string card)
{
    cards_vector_.push_back(card);
}

void Poker::AddCard(const char * card)
{
    std::string tmp(card);
    AddCard(tmp);
}

void Poker::AddCardSet(void)
{
    // heart diamond spade club
    // A 2 3 4 5 6 7 8 9 0 J Q K
    // small joker(SS), big joker(BB)
    const char kNumTable[] = "A234567890JQK";
    const char kColorTable[] = "HDSC";

    AddCard("SS");
    AddCard("BB");
    
    size_t i, j;
    for (i=0; i<strlen(kColorTable); i++) {
        for (j=0; j<strlen(kNumTable); j++) {
            char    card[10];
            sprintf(card, "%c%c", kColorTable[i], kNumTable[j]);
            AddCard(card);
        }
    }
}

bool Poker::TestAndRemoveCardsFromPlayerCards(std::string set, int playerid)
{
    std::string cards = GetPlayerCards(playerid);
    int len = set.length() / 2;
    int i;
    for (i=0; i<len; i++) {
        std::string card = set.substr(i*2, 2);
        size_t  pos = cards.find(card);
        if (pos == std::string::npos)
            return false;
        cards = cards.substr(0, pos) + cards.substr(pos + 2);
    }
    player_cards_[playerid] = cards;
    return true;
}

void Poker::PrintCards(void)
{
    printf("%d cards total.\n", (int)cards_vector_.size());
    std::vector<std::string>::iterator  iter;
    for (iter=cards_vector_.begin(); iter!=cards_vector_.end(); iter++) {
        std::string     tmp = *iter;
        printf("%s ", tmp.c_str());
    }
    printf("\n");
}

Poker::Scene Poker::NextTurn(void)
{
    int players = (int)player_cards_.size();
    if (last_scene_.cards_owner == -1) {
        srand((unsigned int)time(NULL));
        last_scene_.reasonable_player = rand() % players;
    }

    return last_scene_;
}

bool Poker::IsGameOver(void)
{
    int i;
    bool flag1, flag2;
    flag1 = true;
    flag2 = true;

    for (i=0; i<6; i++) {
        if (!GetPlayerCards(i).empty()) {
            if ((i % 2) == 1)
                flag1 = false;
            else
                flag2 = false;
        }
    }
    if (flag1 || flag2)
        return true;
    return false;
}

bool Poker::SetNextReasonablePlayer(void)
{
    do {
        if (rules::IsGouJi(last_scene_.cards)) {
            if (last_scene_.reasonable_player == ((last_scene_.cards_owner + 1) % 6)) {
                last_scene_.reasonable_player = (last_scene_.cards_owner + 5) % 6;
            } else if (last_scene_.reasonable_player == ((last_scene_.cards_owner + 5) % 6)) {
                last_scene_.reasonable_player = (last_scene_.cards_owner + 3) % 6;
            } else if (last_scene_.reasonable_player == ((last_scene_.cards_owner + 3) % 6)) {
                last_scene_.reasonable_player = last_scene_.cards_owner;
                last_scene_.cards = "";
            } else if (last_scene_.cards_owner == last_scene_.reasonable_player) {
                last_scene_.reasonable_player = (last_scene_.reasonable_player + 1) % 6;
            } else {
                return false;
            }
        } else {
            last_scene_.reasonable_player = (last_scene_.reasonable_player + 1) % 6;
        }
        if (last_scene_.reasonable_player == last_scene_.cards_owner && GetPlayerCards(last_scene_.reasonable_player).empty()) {
            last_scene_.cards = std::string("");
            last_scene_.cards_owner = (last_scene_.reasonable_player + 1) % 6;
            last_scene_.reasonable_player = (last_scene_.reasonable_player + 1) % 6; 
        }
        if (IsGameOver())
            return true;
    } while(GetPlayerCards(last_scene_.reasonable_player).empty() || player_states_[last_scene_.reasonable_player].is_failure_);
    return true;
}

bool Poker::Play(std::string set)
{
    if (set.empty()) {
        if (last_scene_.cards_owner == -1 
            || last_scene_.cards.empty() 
            || last_scene_.reasonable_player == last_scene_.cards_owner) {
            return false;
        }
        return SetNextReasonablePlayer();
    } else {
        if (!rules::IsMeaningfulCardSet(set, GetPlayerCards(last_scene_.reasonable_player)))
            return false;

        if (last_scene_.cards_owner == -1 || last_scene_.cards.empty() || last_scene_.reasonable_player == last_scene_.cards_owner) {
            if (!TestAndRemoveCardsFromPlayerCards(set, last_scene_.reasonable_player))
                return false;
            last_scene_.cards = set;
            last_scene_.cards_owner = last_scene_.reasonable_player;
            return SetNextReasonablePlayer();
        }

        if (!rules::IsSet1BiggerThanSet2(set, last_scene_.cards))
            return false;

        if (!TestAndRemoveCardsFromPlayerCards(set, last_scene_.reasonable_player))
            return false;
        
        int owner = last_scene_.cards_owner;
        int player = last_scene_.reasonable_player;    

        if (player_states_[owner].shaoing == PLAYERSTATES::SHAO_SHAOING) {
            player_states_[owner].shaoing = PLAYERSTATES::SHAO_NORMAL;
            player_states_[owner].shao_failure_times_++;
        }

        if (player_states_[player].shaoing == PLAYERSTATES::SHAO_SHAOING) {
            if (!rules::IsShaoingSet(set)) {
                player_states_[player].is_failure_ = true;
                last_scene_.cards = "";
                last_scene_.cards_owner = -1;
                last_scene_.reasonable_player = (owner + 3) % 6;
            }
        }

        if (owner != player && !IsSeatOpposite(owner, player) && rules::IsGouJi(last_scene_.cards)) {
            player_states_[player].shaoing = PLAYERSTATES::SHAO_SHAOING;
        }
        
        last_scene_.cards = set;
        last_scene_.cards_owner = last_scene_.reasonable_player;
        return SetNextReasonablePlayer();
    }

    return false;
}

std::string Poker::SortCards(std::string& set)
{
    return rules::SortCards(set);
}

bool Poker::IsSeatOpposite(int seat1, int seat2)
{
    if (seat1 < 0 || seat1 >= 6 || seat2 < 0 || seat2 >= 0)
        return false;

    if ( ((6 + seat1 - seat2) % 6) == 3)
        return true;
    return false;
}


#if 0
void TestShuffle(Poker& poker)
{/*{{{*/
    poker.PrintCards();
    poker.Shuffle();
    poker.PrintCards();

}/*}}}*/

void TestSort(Poker& poker)
{/*{{{*/
    std::string cards("3344");
    std::string res = rules::SortCards(cards);
    printf("res = %s\n", res.c_str());
}/*}}}*/

void TestCompare(Poker& poker)
{/*{{{*/
    int ret = rules::CompareCards(std::string("SS"), std::string("BB"));
    printf("SS vs BB = %d\n", ret);
}/*}}}*/

void TestDeal(Poker& poker)
{/*{{{*/
    int     i;
    poker.Deal(6);
    for (i=0; i<6; i++) {
        printf("Player%d: %s\n", i, poker.GetPlayerCards(i).c_str());
        std::string sorted = rules::SortCards(poker.GetPlayerCards(i));
        printf("sorted : %s\n", sorted.c_str());
    }
}/*}}}*/

void TestChecker(Poker& poker)
{/*{{{*/
#define TESTRULE(AJ, A2, s) \
    { \
        std::string set(s); \
        rules::allow_joker_replacing = AJ; \
        rules::allow_2_replacing = A2; \
        bool ret = rules::IsMeaningfulCardSet(set); \
        printf("J(%d) 2(%d) %s = %d\n", AJ, A2, s, ret); \
    }

    TESTRULE(false, false, "H6C6D6S6");
    TESTRULE(false, false, "H6H7");
    TESTRULE(false, false, "H6SSBB");

    TESTRULE(true, false,  "H6C6D6S6");
    TESTRULE(true, false,  "H6H7");
    TESTRULE(true, false,  "H6S6SS");
    TESTRULE(true, false,  "H6S6SSBB");

    TESTRULE(true, false,  "H6S6H2SSBB");
    TESTRULE(true, true,  "H6S6H2SSBB");
    TESTRULE(true, true,  "H6S6H2C2D2");
#undef TESTRULE
}/*}}}*/

void TestSetCompare(Poker& poker)
{/*{{{*/
    rules::allow_joker_replacing = true;
    rules::allow_2_replacing = true;
    rules::allow_222_bigger_than_big_joker = true;
#define TESTC(s2, s1) \
    { \
        std::string set1(s1); \
        std::string set2(s2); \
        bool ret = rules::IsSet1BiggerThanSet2(set1, set2); \
        printf("set1(%s) > set2(%s) = %d\n", s1, s2, ret); \
    }

    TESTC("H6H6H6H6S2S2", "H7H7H7H7H7H7");
    TESTC("H6H6H6H6S2SS", "H7H7H7H7H7H7");
    TESTC("H6H6H6H6S2SS", "H7H7H7H7H7SS");
    TESTC("H6H6H6H6S2SS", "H7H7H7H7H7BB");
    TESTC("H6H6H6H6SSBB", "H7H7H7H7BBBB");
    TESTC("H6H6H6H6SSBB", "H7H7H7H7BBH2H2H2");
    TESTC("BB", "D2H2H2");
#undef TESTC
}/*}}}*/

void TestGouJi(Poker& poker)
{/*{{{*/
#define TESTC(s, result) \
    { \
        std::string set(s); \
        bool ret = rules::IsGouJi(set); \
        printf("set(%s) is ", s); \
        if (!ret) { \
            printf("NOT "); \
        } \
        printf("GouJi... test success = %d\n", !(result ^ ret)); \
    }

    TESTC("H0H0H0H0", false);
    TESTC("H0H0H0H0H0", true);
    TESTC("HJHJHJHJHJ", true);
    TESTC("HJHJHJH2H2", true);
    TESTC("H6H6H6H6H6", false);
    TESTC("H6H6H6H6SS", true);
    TESTC("H6H6H6H6BB", true);
    TESTC("HA", false);
    TESTC("HAHA", true);
    TESTC("H2", true);
    TESTC("H2H2", true);

#undef TESTC

}/*}}}*/

int main()
{
    Poker   poker;
    TestShuffle(poker);
    TestCompare(poker);
    TestSort(poker);
    TestDeal(poker);
    TestChecker(poker);
    TestSetCompare(poker);
    TestGouJi(poker);

    char            buffer[1024];
    Poker::Scene    scene;
    while (true) {
        scene = poker.NextTurn();
        printf("-----------------------------\n");
        printf("the cards is: %s\n", scene.cards.c_str());
        printf("owner's id is: %d\n", scene.cards_owner);
        printf("now the reasonable player is: %d\n", scene.reasonable_player);
        printf("now it's your turn. your cards are:\n%s\n", rules::SortCards(poker.GetPlayerCards(scene.reasonable_player)).c_str());

        bool flag = true;
        while (flag) {
            printf("input PASS or PLAY + cards:\n");
            memset(buffer, 0, sizeof(buffer));
            fgets(buffer, 1023, stdin);
            buffer[strlen(buffer) - 1] = '\0';
            if (strncasecmp(buffer, "PASS", 4) == 0) {
                if (poker.Pass())
                    flag = false;
            } else if (strncasecmp(buffer, "PLAY", 4) == 0) {
                std::string line(buffer+4);
                line = util::Trim(line);
                if (!poker.Play(line)) {
                    printf("failed. try again.\n");
                } else {
                    flag = false;
                }
            } else {
            }
        }
    }
    return 0;
}
#endif


