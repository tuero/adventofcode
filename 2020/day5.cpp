#include <iostream>
#include <vector>
#include <unordered_set>
#include <limits>           // numeric_limits

#include "common.h"


/**
 * Find the number from string ID using char to specifiy high bit
 * @param lines Vector of strings, each element is a line from stdin
 * @param high_bit_char Char being treated as high bit
 * @return Binary space partition number
 */
uint64_t calc_num(const std::string &id, const char high_bit_char) {
    uint64_t num = 0;
    uint64_t mask = 0;

    // Calculate the row by setting bit to high if B
    for (auto it = id.crbegin() ; it != id.crend(); ++it) {
        if (*it == high_bit_char) {
            num |= (1UL << mask);
        }
        ++mask;
    }
    return num;
}


uint64_t get_row(const std::string &line) {
    return calc_num(line.substr(0, 7), 'B');
}


uint64_t get_col(const std::string &line) {
    return calc_num(line.substr(7), 'R');
}


/**
 * Find the max seat ID
 * @param lines Vector of strings, each element is a line from stdin
 * @return Maximum seat ID
 */
long long int solution1(std::vector<std::string> &lines) {
    long long int max_id = 0;

    for (const auto & line : lines) {
        long long int id = get_row(line) * 8 + get_col(line);
        if (id > max_id) {max_id = id;}
    }

    return max_id;
}


/**
 * Find the correct seat ID as defined as the missing number which is occupied by +/- 1
 * @param lines Vector of strings, each element is a line from stdin
 * @return Correct seat ID
 */
int solution2(std::vector<std::string> &lines) {
    long long int min_id = std::numeric_limits<int>::max(), max_id = 0;
    std::unordered_set<long long int> ids;

    // Populate known seat ids
    for (const auto & line : lines) {
        long long int id = get_row(line) * 8 + get_col(line);
        ids.insert(id);
        if (id > max_id) {max_id = id;}
        if (id < min_id) {min_id = id;}
    }

    // Find missing ID
    for (long long int id = min_id + 1; id < max_id; ++id) {
        if (ids.find(id) == ids.end()) {
            return id;
        }
    }

    return -1;
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    int id1 = solution1(lines);
    std::cout << "Highest seat ID in part 1: " << id1 << std::endl;
    int id2 = solution2(lines);
    std::cout << "Correct seat ID in part 2: " << id2 << std::endl;
}