#include <iostream>
#include <string>
#include <vector>
#include <algorithm>        // count
#include <cassert>

#include "common.h"


// consts
const std::string DELIMS = " -:";


int solution1(std::vector<std::string> &lines) {
    int count = 0;
    for (auto line : lines) {
        size_t iter_begin = 0, iter_curr = 0;
        std::vector<std::string> line_data;

        // Extract needed information
        while ((iter_begin = line.find_first_not_of(DELIMS, iter_curr)) != std::string::npos) {
            iter_curr = line.find_first_of(DELIMS, iter_begin + 1);
            line_data.push_back(line.substr(iter_begin, iter_curr - iter_begin));
        }

        // Convert to needed format
        int min = std::stoi(line_data[0]);
        int max = std::stoi(line_data[1]);
        char rule = line_data[2][0];
        std::string &password = line_data[3];
        std::size_t occurances = std::count(password.begin(), password.end(), rule);

        // Password matches rule
        if (min <= occurances && occurances <= max) {
            ++count;
        }
    }

    return count;
}


int solution2(std::vector<std::string> &lines) {
    int count = 0;
    for (auto line : lines) {
        size_t iter_begin = 0, iter_curr = 0;
        std::vector<std::string> line_data;

        // Extract needed information
        while ((iter_begin = line.find_first_not_of(DELIMS, iter_curr)) != std::string::npos) {
            iter_curr = line.find_first_of(DELIMS, iter_begin + 1);
            line_data.push_back(line.substr(iter_begin, iter_curr - iter_begin));
        }

        // Convert to needed format
        size_t pos1 = std::stoi(line_data[0]) - 1;
        size_t pos2 = std::stoi(line_data[1]) - 1;
        char rule = line_data[2][0];
        std::string &password = line_data[3];

        // Password matches rule
        assert (pos1 < password.size() && pos2 < password.size());
        if ((password[pos1] == rule) ^ (password[pos2] == rule)) {
            ++count;
        }
    }

    return count;
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    int count1 = solution1(lines);
    std::cout << "Number of valid passwords part 1: " << count1 << std::endl;
    
    int count2 = solution2(lines);
    std::cout << "Number of valid passwords part 2: " << count2 << std::endl;
}