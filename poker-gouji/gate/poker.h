

#pragma once

#include <string>
#include <vector>

class Poker {
public:
    Poker();
    ~Poker();

    // Ï´
    void Shuffle(void);

    // ·¢
    void Deal(int players = 6);
    std::string GetPlayerCards(int player);

    // ÂÖ
    class Scene {
    public:
        Scene() : cards(""), cards_owner(-1), reasonable_player(-1) {}
        ~Scene() {}
        Scene(const Scene& o) 
            : cards(o.cards.c_str())
            , cards_owner(o.cards_owner)
            , reasonable_player(o.reasonable_player) {
        }
        Scene& operator=(const Scene& o) {
            cards = std::string(o.cards.c_str());
            cards_owner = o.cards_owner;
            reasonable_player = o.reasonable_player;
            return *this;
        }
        void Clear() {
            cards = std::string("");
            cards_owner = -1;
            reasonable_player = -1;
        }
        std::string     cards;
        int             cards_owner;
        int             reasonable_player;
    };

    Scene NextTurn(void);

    bool Play(std::string cards = std::string(""));
    bool Pass(void) { return Play(); }

    bool IsGameOver(void);

public:
    // for test
    void PrintCards(void);

private:
    void AddCard(std::string card);
    void AddCard(const char * card);
    void AddCardSet(void);

private:
    std::vector<std::string>    cards_vector_;    
    std::vector<std::string>    player_cards_;

public:    
    typedef struct tagPLAYERSTATES {
        tagPLAYERSTATES() : dianing(DIAN_FAILURE), mening(MEN_NOT), shaoing(SHAO_NORMAL), shao_failure_times_(0), is_failure_(false) {}
        enum {
            DIAN_FAILURE,
            DIAN_TRYING,
            DIAN_SUCCESS,
        } dianing; 
        enum {
            MEN_NOT,
            MEN_ENSURE,
        } mening;
        enum {
            SHAO_NORMAL,
            SHAO_SHAOING,
            SHAO_SUCCESS,
        } shaoing; 
        int shao_failure_times_;
        bool is_failure_;
    } PLAYERSTATES;

    std::vector<PLAYERSTATES>   player_states_;
    bool IsSeatOpposite(int seat1, int seat2);

public:
    Scene                       last_scene_;

private:
    bool SetNextReasonablePlayer(void);
    bool TestAndRemoveCardsFromPlayerCards(std::string set, int playerid);

public:
    std::string SortCards(std::string& set);

};

