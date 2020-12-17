#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>            // count_if
#include <cassert>

#include "common.h"



/**
 * Gets a map of question counts
 * @param lines Vector of strings, each element is a line from stdin
 * @param i Current index in the lines vector
 * @return Map of question counts
 */
std::unordered_map<char, int> get_question_map(std::vector<std::string> &lines, size_t &i) {
    std::unordered_map<char, int> question_map;

    // Insert new questions
    while (lines[i] != "") {
        for (const auto c : lines[i]) {
            ++question_map[c];
        }
        ++i;
    }

    return question_map;
}


/**
 * Gets the sum of unique questions
 * @param lines Vector of strings, each element is a line from stdin
 * @return Sum of unique questions
 */
long long int solution1(std::vector<std::string> &lines) {
    long long int count = 0;

    for (std::size_t i = 0; i < lines.size(); ++i) {
        // Get question set
        std::unordered_map<char, int> question_map = get_question_map(lines, i);
        count += question_map.size();
    }

    return count;
}


/**
 * Gets the sum of unique questions to which everyone answered
 * @param lines Vector of strings, each element is a line from stdin
 * @return Sum of unique questions
 */
long long int solution2(std::vector<std::string> &lines) {
    long long int count = 0;

    for (std::size_t i = 0; i < lines.size(); ++i) {
        int j = i;
        // Get question set
        std::unordered_map<char, int> question_map = get_question_map(lines, i);
        int number_people = i - j;
        count += std::count_if(question_map.begin(), question_map.end(), [&](std::pair<char, int> it) {
            return it.second == number_people;
        });
    }

    return count;
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    int count1 = solution1(lines);
    std::cout << "Sum of counts in part 1: " << count1 << std::endl;
    int count2 = solution2(lines);
    std::cout << "Sum of counts in part 2: " << count2 << std::endl;
}