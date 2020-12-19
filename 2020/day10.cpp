#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>            // sort

#include "common.h"


/**
 * Get the sorted list of jolts for adapters
 * @param lines Vector of strings, each element is a line from stdin
 * @return List of jolts
 */
std::vector<std::size_t> get_sorted_input(std::vector<std::string> &lines) {
    std::vector<std::size_t> nums;

    // Get input and sort
    for (const auto & line : lines) {
        nums.push_back(std::stoll(line));
    }
    std::sort(nums.begin(), nums.end());
    nums.push_back(nums[nums.size() - 1] + 3);

    return nums;
}


/**
 * Get the map of jolt differences
 * @param lines Vector of strings, each element is a line from stdin
 * @return The map containing jolt difference counts
 */
std::unordered_map<std::size_t, int> get_difference_map(std::vector<std::string> &lines) {
    // Get input and sort
    std::vector<std::size_t> nums = get_sorted_input(lines);

    // Find differences
    std::size_t prev = 0;
    std::unordered_map<std::size_t, int> difference_map;
    for (const auto & num : nums) {
        ++difference_map[num - prev];
        prev = num;
    }

    return difference_map;
}

/**
 * Finds the difference in jolts (1-jolt * 3-jolt)
 * @param lines Vector of strings, each element is a line from stdin
 * @return The product of 1/3 jolt differences
 */
std::size_t solution1(std::vector<std::string> &lines) {
    std::unordered_map<std::size_t, int> difference_map = get_difference_map(lines);

    if (difference_map.find(1) == difference_map.end() || difference_map.find(3) == difference_map.end()) {
        throw "Invalid input";
    }
    return difference_map[1] * difference_map[3];
}


/**
 * Finds the number of possible adapter configurations
 * @param lines Vector of strings, each element is a line from stdin
 * @return The number of valid configurations
 */
std::size_t solution2(std::vector<std::string> &lines) {
    // Get input and sort
    std::vector<std::size_t> nums = get_sorted_input(lines);

    // Track number of paths to reach each option in the jolt list
    std::unordered_map<std::size_t, std::size_t> path_tracker = {{0, 1}};
    const std::array<int, 3> deltas = {1, 2, 3};
    for (const auto & num : nums) {
        if (path_tracker.find(num) != path_tracker.end()) {
            continue;
        }
        // Cache how many ways we could have gotten here
        for (const auto & delta : deltas) {
            // Might not be in path_tracker, but default construct to 0 doesn't affect sum
            path_tracker[num] += path_tracker[num-delta];
        }
    }

    return path_tracker[nums[nums.size() - 1]];
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    std::size_t num1 = solution1(lines);
    std::cout << "Jolt difference output in part 1: " << num1 << std::endl;
    std::size_t num2 = solution2(lines);
    std::cout << "Number of combinations in part 2: " << num2 << std::endl;
}