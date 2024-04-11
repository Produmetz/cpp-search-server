#pragma once
#include <iostream>


const int MAX_RESULT_DOCUMENT_COUNT = 5;
const double EPSILON = 1e-6;

struct Document {
    Document() = default;

    Document(int id, double relevance, int rating)
        : id(id)
        , relevance(relevance)
        , rating(rating) {
    }

    int id = 0;
    double relevance = 0.0;
    int rating = 0;
};
enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};
std::ostream& operator<<(std::ostream& out, const Document& document);