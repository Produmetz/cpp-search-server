#include "read_input_functions.h"

using namespace std::string_literals;
std::string ReadLine() {
    std::string str;
    std::getline(std::cin, str);
    return str;
}
int ReadLineWithNumber() {
    int result;
    std::cin >> result;
    ReadLine();
    return result;
}
std::ostream& operator<<(std::ostream& out, const Document& document) {
    //std::string_literals::s;
    out <<"{ "s
        << "document_id = "s << document.id << ", "s
        << "relevance = "s << document.relevance << ", "s
        << "rating = "s << document.rating <<" }"s;
    return out;
}
