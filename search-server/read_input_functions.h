#pragma once
#include <string>
#include <iostream>
#include "paginator.h"
#include "document.h"

std::string ReadLine();

int ReadLineWithNumber();



template <typename Iterator>
std::ostream& operator<<(std::ostream& out, const IteratorRange<Iterator>& range) {
    for (Iterator it = range.begin(); it != range.end(); ++it) {
        out << *it;
    }
    return out;
}
std::ostream& operator<<(std::ostream& out, const Document& document);