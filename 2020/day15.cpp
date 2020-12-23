#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>

#include "common.h"


// consts
const int DURATION1 = 2020;
const int DURATION2 = 30000000;

/**
 * Plays the memory game up to the duraction
 * @param lines Vector of ints from stdin
 * @param duration The duration of the game
 * @return The last number spoken
 */
int play_game(const std::vector<int> & numbers, int duration) {
    std::unordered_map<int, int> counter_map;
    int counter = 0;
    int last_number = -1;
    int last_idx = -1;
    bool new_number = true;

    // Run one round of speaking game
    auto run_round = [&](int number) {
        new_number = (counter_map.find(number) == counter_map.end());
        if (!new_number) {
            last_idx = counter_map[number];
        }
        counter_map[number] = ++counter;
        last_number = number;
    };

    // Populate initial numbers
    for (const auto & number : numbers) {
        run_round(number);
    }
    
    // Continue 
    while (counter < duration) {
        int number = (new_number ? 0 : counter_map[last_number] - last_idx);
        run_round(number);
    }
    return last_number;
}

/**
 * Gets the 2020th number spoken
 * @param lines Vector of ints from stdin
 * @return 2020th number spoken
 */
int solution1(const std::vector<int> & numbers) {
    return play_game(numbers, DURATION1);
}


/**
 * Gets the 30000000th number spoken
 * @param lines Vector of ints from stdin
 * @return 30000000th number spoken
 */
int solution2(const std::vector<int> & numbers) {
    return play_game(numbers, DURATION2);
}


int main() {
    // Get data from stdin
    std::vector<int> numbers = common::read_stdin<int>(',');

    uint64_t num1 = solution1(numbers);
    std::cout << "Number spoken in part 1: " << num1 << std::endl;
    uint64_t num2 = solution2(numbers);
    std::cout << "Number spoken in part 2: " << num2 << std::endl;
}