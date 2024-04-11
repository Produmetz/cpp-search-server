#pragma once
#include <string>
#include <vector>
#include <set>

bool IsValidWord(const std::string& word);

std::vector<std::string> SplitIntoWords(const std::string& text);

template <typename StringContainer>
std::set<std::string> MakeUniqueNonEmptyStrings(const StringContainer& strings);