#include <iostream>
#include <sstream>
#include <iterator>
#include <array>
#include <vector>
#include <string>
#include <unordered_set>

#include "common.h"


/**
 * Runs the program, and terminates if a loop is found or the program terminates
 * @param lines Vector of strings, each element is a line from stdin
 * @param loop_flag reference to store whether returned value signifies accumulator or loop
 * @return The accumulator count before repetition
 */
long long int run_program(std::vector<std::string> &lines, int &loop_flag) {
    long long int accumulator = 0;
    std::unordered_set<std::size_t> instruction_tracker;
    std::size_t i = 0;
    loop_flag = 0;

    while (i < lines.size()) {
        // Split string by space
        std::istringstream iss(lines[i]);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

        // Check if repeated instruction, and store
        if (instruction_tracker.find(i) != instruction_tracker.end()) {
            loop_flag = 1;
            return accumulator;
        }
        instruction_tracker.insert(i);

        // Handle instruction type
        if (tokens[0] == "acc") {
            // Accumulate by given amount
            int dir = (tokens[1][0] == '+') ? 1 : -1;
            accumulator += (dir * std::stoi(tokens[1].substr(1)));
        } else if (tokens[0] == "jmp") {
            // Jump forward/backward to instruction
            int dir = (tokens[1][0] == '+') ? 1 : -1;
            i += (dir * std::stoi(tokens[1].substr(1))) - 1;           
        }
        ++i;
    }
    return accumulator;
}


/**
 * Gets the accumulator count before code execution repeats
 * @param lines Vector of strings, each element is a line from stdin
 * @return The accumulator count before repetition
 */
long long int solution1(std::vector<std::string> &lines) {
    int loop_flag = 0;
    long long int accumulator = run_program(lines, loop_flag);

    // We expect the program to hit a loop
    if (!loop_flag) {throw "Expected loop";}
    return accumulator;
}


/**
 * Changes one NOP to JMP or vise-versa so that the program eventually terminates
 * @param lines Vector of strings, each element is a line from stdin
 * @return The accumulator count before repetition
 */
long long int solution2(std::vector<std::string> &lines) {
    int loop_flag = 0;
    long long int accumulator = 0;
    const std::string str_nop = "nop", str_jmp = "jmp";
    std::size_t index;
    std::vector<std::array<std::string, 2>> swaps = {
        {str_nop, str_jmp},
        {str_jmp, str_nop},
    };
    
    // Trivially try each change NOP/JMP
    for (std::size_t i = 0; i < lines.size(); ++i) {
        std::string & line = lines[i];
        // Try each swapping rule
        for (const auto & swap : swaps) {
            const std::string & from = swap[0], & to = swap[1];
            // Line contains swap rule
            if ((index = line.find(from)) != std::string::npos) {
                line.replace(index, from.size(), to);
                accumulator = run_program(lines, loop_flag);
                line.replace(index, to.size(), from);
                if (!loop_flag) {
                    return accumulator;
                }
            }
        }
    }

    throw "No swap found";
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    int count1 = solution1(lines);
    std::cout << "Accumulator count in part 1: " << count1 << std::endl;
    int count2 = solution2(lines);
    std::cout << "Accumulator count in part 2: " << count2 << std::endl;
}