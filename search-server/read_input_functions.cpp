#include "read_input_functions.h"

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
