#include "string_processing.h"
#include <stdexcept>
#include <algorithm>
using namespace std::string_literals;

bool IsValidWord(const std::string& word) {
    // A valid word must not contain special characters
    return std::none_of(word.begin(), word.end(), [](char c) {
        return c >= '\0' && c < ' ';
    });
}
std::vector<std::string> SplitIntoWords(const std::string& text) {
    std::vector<std::string> words;
    std::string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                if(!IsValidWord(word)){
                    throw std::invalid_argument("special symbol, word : "s + word);
                }
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        if(!IsValidWord(word)){
            throw std::invalid_argument("special symbol, word : "s + word);
        }
        words.push_back(word);
    }

    return words;
}
