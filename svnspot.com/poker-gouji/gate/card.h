
#pragma once

#include <string>

// Heart Club Spade Diamond
// A 2 3 4 5 6 7 8 9 0 J Q K
// Small Joker, Big Joker

class Card {
public:
    Card(const char * card) : card_(card) {}
    Card(std::string& card) : card_(card) {}
    Card(const Card& o) : card_(o.card_) {}
    ~Card();
    std::string name(void) { return card_; }
private:
    std::string card_;

public:
    Card& operator=(const Card& o) {
        card_ = o.card_;
        return *this;
    }

    bool operator>(const Card& o);
    bool operator<(const Card& o);
    bool operator==(const Card& o);

    bool operator>=(const Card& o) { return !(operator<(o)); }
    bool operator<=(const Card& o) { return !(operator>(o)); }


};

