

#include "gouji.h"
#include <assert.h>
#include <vector>
#include <string.h>

namespace rules {

bool    allow_joker_replacing = true;
bool    allow_2_replacing = true;
bool    allow_222_bigger_than_big_joker = true;
bool    keep_3_until_endding = true;
bool    allow_joker_taking_replace_of_3 = false;

char table[] = "34567890JQKA2SB";

static int GetTableOffset(char c) {
    char* p = strchr(table, c);
    if (p)
        return p - table;
    else
        return -1;
}

static std::string DropColor(std::string& cards)
{
    int i;
    std::string tmp("");
    int len = cards.length();
    for (i=1; i<len; i+=2) {
        tmp += cards.substr(i, 1);
    }
    return tmp; 
}

static int GetBaseOfSet(std::string set)
{
    int sums[sizeof(rules::table)] = {0};
    int len;
    int i;
    memset(sums, 0, sizeof(sums));
    len = set.length();
    for (i=0; i<len; i++) {
        char c = set.at(i);
        char* p = strchr(rules::table, c);
        assert(p != NULL);
        if (p == NULL)
            return -1;
        sums[p - rules::table]++;
    }
    len = strlen(rules::table);
    for (i=0; i<len; i++) {
        if (sums[i] > 0)
            return i;
    }
    return -1;
}

static int GetCardsCount(std::string set, char card)
{
    int     sum =0;
    size_t  i;
    for (i=0; i<set.length(); i++) {
        if (set.at(i) == card)
            sum++;
    }
    return sum;
}


// the following fucntions should be used by class Card/CardSet.

std::string SortCards(std::string cards)
{
    int count = cards.length() / 2;
    int i, j;
    std::string tmp;
    std::vector<std::string>    v;
    for (i=0; i<count; i++) {
        tmp = cards.substr(i*2, 2);
        v.push_back(tmp);
    }

    std::string card1, card2;
    for (j=count-1; j > 0; j--) {
        for (i=0; i<j; i++) {
            if (CompareCards(v[i], v[i+1]) < 0) {
                tmp = v[i];
                v[i] = v[i+1];
                v[i+1] = tmp;
            }
        }
    }
    
    tmp = "";
    for (i=0; i<count; i++) {
        tmp += v[i];
    }
    return tmp;
}

int CompareCards(std::string card1, std::string card2)
{
    char c1 = card1.at(1);
    char c2 = card2.at(1);
    const char * p1 = strchr(rules::table, c1);
    const char * p2 = strchr(rules::table, c2);
    if (p1 == NULL || p2 == NULL) {
        throw ("Not a card!");
    }
    return p1 - p2;
}

static bool _check3(std::string set, std::string handcards)
{
    int i;
    if (keep_3_until_endding) {
        set = DropColor(set);
        int c = GetBaseOfSet(set);
        if (rules::table[c] == '3') {
            int _card_count[sizeof(rules::table)];
            memset(_card_count, 0, sizeof(_card_count));
            int count = handcards.length() / 2;
            for (i=0; i<count; i++) {
                std::string card = handcards.substr(i*2, 2);
                char c = card.at(1);
                char * p = strchr(rules::table, c);
                if (p != NULL) {
                    _card_count[p - rules::table] ++;
                }
            }
            int kinds = 0;
            for (i=0; i<(int)(sizeof(_card_count)/sizeof(_card_count[0])); i++) {
                if (_card_count[i] > 0)
                    kinds++;
            }
            // one kind of cards;
            if (kinds != 1)
                return false;
        }
    }
    return true;
}

bool IsMeaningfulCardSet(std::string set, std::string handcards)
{
    int count = set.length() / 2;
    int i;
    std::string card;

    int _card_count[sizeof(rules::table)];
    memset(_card_count, 0, sizeof(_card_count));

    for (i=0; i<count; i++) {
        card = set.substr(i*2, 2);
        char c = card.at(1);
        char * p = strchr(rules::table, c);
        if (p != NULL) {
            _card_count[p - rules::table] ++;
        }
    }

    int kinds = 0;
    for (i=0; i<(int)(sizeof(_card_count)/sizeof(_card_count[0])); i++) {
        if (_card_count[i] > 0)
            kinds++;
    }

    // one kind of cards;
    if (kinds == 1)
        return _check3(set, handcards);

    int ps = -1, pb = -1, p2 = -1;
#define FINDOFFSET(c, v) {char*p = strchr(rules::table, c); if(!p)throw("!!"); v = p - rules::table;}
    FINDOFFSET('S', ps);
    FINDOFFSET('B', pb);
    FINDOFFSET('2', p2);
#undef FINDOFFSET
    
    // two kinds of jokers.
    if (kinds == 2) {
        if (_card_count[ps] > 0 && _card_count[pb] > 0)
            return _check3(set, handcards);
    }

    if (rules::allow_joker_replacing) {
        if (_card_count[ps] > 0)
            kinds --;
        if (_card_count[pb] > 0)
            kinds --;
    }
    
    if (kinds == 1)
        return _check3(set, handcards);

    if (rules::allow_2_replacing) {
        if (_card_count[p2] > 0)
            kinds --;
    }

    if (kinds != 1)
        return false;

    return _check3(set, handcards);
}

bool IsSet1BiggerThanSet2(std::string set1, std::string set2)
{
    int base1 = 0, base2 = 0;
    int ss2 = 0, bb1 = 0, bb2 = 0;

    set1 = DropColor(set1);
    set2 = DropColor(set2);
 
    base1 = GetBaseOfSet(set1);
    base2 = GetBaseOfSet(set2);

    if (base1 < 0 || base2 < 0)
        return false;

    if (base1 <= base2) {
        if (!rules::allow_222_bigger_than_big_joker)
            return false;
        if (rules::table[base1] != '2' || rules::table[base2] != 'B') {
            return false;
        }
    }

    bb1 = GetCardsCount(set1, 'B');
    ss2 = GetCardsCount(set2, 'S');
    bb2 = GetCardsCount(set2, 'B');

    if (bb1 < ss2)
        return false;

    if (set1.length() == set2.length()) {
        if (bb2 > 0)
            return false;
    } else {
        if (!rules::allow_222_bigger_than_big_joker)
            return false;
        if (bb2 <= 0)
            return false;

        if (set1.length() - bb2 * 2 != set2.length())
            return false;

        int c2 = GetCardsCount(set1, '2');
        if (c2 < bb2 * 3)
            return false;
    }
    return true;
}

bool IsGouJi(std::string set)
{
    if (set.empty())
        return false;

    set = DropColor(set);
    int bb = 0, ss = 0;
    ss = GetCardsCount(set, 'S');
    bb = GetCardsCount(set, 'B');
    if (ss > 0 || bb > 0)
        return true;

    int base = GetBaseOfSet(set);
    if (rules::table[base] == '2')
        return true;

    int pos10 = rules::GetTableOffset('0');
    assert(pos10 > 0);

    if (base < pos10)
        return false;

    if (base == rules::GetTableOffset('A')) {
        if (set.length() >= 2)
            return true;
        else
            return false;
    }
    
    int need = 5 - (base - pos10);
    if ((int)set.length() >= need)
        return true;
    
    return false;
}

bool IsShaoingSet(std::string set)
{
    set = DropColor(set);
    int ss = GetCardsCount(set, 'S');
    int bb = GetCardsCount(set, 'B');
    if (ss > 0 || bb > 0)
        return true;
    return false;
}


} // namespace rules

