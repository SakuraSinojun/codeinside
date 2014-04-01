

#pragma once


#include <string>

namespace rules {

extern bool    allow_joker_replacing;
extern bool    allow_2_replacing;
extern bool    allow_222_bigger_than_big_joker;
extern bool    keep_3_until_endding;
extern bool    allow_joker_taking_replace_of_3;

std::string SortCards(std::string cards);
int CompareCards(std::string card1, std::string card2);
bool IsSet1BiggerThanSet2(std::string set1, std::string set2);
bool IsMeaningfulCardSet(std::string set, std::string handcards = std::string(""));


bool IsGouJi(std::string set);

bool IsShaoingSet(std::string set);


} // namespace rules;

