#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>    // find_if
#include <stdlib.h>     // exit


namespace common {

/**
 * Read in a single 
 */
template <typename T>
std::vector<T> read_stdin(const char delimiter = ' ') {
    std::vector<T> inputs;
    T input;

    while (std::cin >> input) {
        inputs.push_back(input);
        if (std::cin.peek() == delimiter) {
            std::cin.ignore();
        }
    }

    if (!std::cin.eof()) {
        std::cerr << "Error proccessing input." << std::endl;
        exit(1);
    }

    return inputs;
}

/**
 * Read in lines from stdin
 * @returns Vector of strings, each element represents an input line
 */
std::vector<std::string> read_stdin_lines() {
    std::vector<std::string> inputs;
    for (std::string line; std::getline(std::cin, line);) {
        inputs.push_back(line);
    }

    return inputs;
}


/**
 * Split a string by a given delimiter
 * @param s The input string
 * @param delim The delimiter to split by
 * @return Vector of string split by delim
 */
std::vector<std::string> split(const std::string & s, char delim = ' ') {
    std::vector<std::string> elems;
    auto result = std::back_inserter(elems);
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }

    return elems;
}


/**
 * Checks if a given string is a (positive) integer
 * @param s The input string
 * @return True if the input string is a (positive) integer
 */
bool is_number(const std::string & s) {
    auto is_digit = [](unsigned char c) { return !std::isdigit(c); };
    return (!s.empty() && std::find_if(s.begin(), s.end(), is_digit) == s.end());
}

} // namespace common

