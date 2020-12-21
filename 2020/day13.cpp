#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>

#include "common.h"


/**
 * Gets the bus ID multiplied by the time waiting
 * @param lines Vector of strings, each element is a line from stdin
 * @return Bus ID multiplied by the time waiting
 */
std::size_t solution1(const std::vector<std::string> &lines) {
    assert (lines.size() == 2);
    int timestamp = std::stoll(lines[0]);
    std::vector<int> ids;
    std::stringstream s_stream(lines[1]);

    // Get known interval ids
    while(s_stream.good()) {
        std::string substr;
        std::getline(s_stream, substr, ',');
        if (substr != "x") {
            ids.push_back(std::stoi(substr));
        }
    }
    
    // Find min waiting time
    int min_time = std::numeric_limits<int>::max();
    int min_id = -1;
    for (const auto & id : ids) {
        std::cout << id;
        int time_to_wait = id - (timestamp % id);
        if (time_to_wait < min_time) {
            min_id = id;
            min_time = time_to_wait;
        }
    }

    assert (min_id > 0);
    return min_id * min_time;
}


/**
 * Gets the bus ID multiplied by the time waiting
 * @param lines Vector of strings, each element is a line from stdin
 * @return Bus ID multiplied by the time waiting
 */
std::size_t solution2(const std::vector<std::string> &lines) {
    assert (lines.size() == 2);
    std::vector<int> ids;
    std::vector<int> offsets;
    std::stringstream s_stream(lines[1]);

    // Get known interval ids
    int offset = 0;
    while(s_stream.good()) {
        std::string substr;
        std::getline(s_stream, substr, ',');
        if (substr != "x") {
            ids.push_back(std::stoi(substr));
            offsets.push_back(offset);
        }
        ++offset;
    }

    // Jump in increments that work
    std::size_t increment = ids[0];
    std::size_t candidate_time = ids[0];
    for (std::size_t i = 1; i < ids.size(); ++i) {
        while ((candidate_time + offsets[i]) % ids[i] != 0) {
            candidate_time += increment;
        }
        increment *= ids[i];
    }

    return candidate_time;
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    std::size_t num1 = solution1(lines);
    std::cout << "Bus in part 1: " << num1 << std::endl;
    std::size_t num2 = solution2(lines);
    std::cout << "Bus in part 2: " << num2 << std::endl;
}