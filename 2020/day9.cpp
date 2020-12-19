#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <unordered_map>

#include "common.h"


// consts
const size_t BUFFER_SZ = 25;


/**
 * Maps the pairwise sums
 * @param sum_map Reference to map to fill
 * @param buffer Items to sum
 */
void set_sum_pair(std::unordered_map<std::size_t, int> &sum_map, std::array<long long int, BUFFER_SZ> &buffer) {
    sum_map.clear();
    for (std::size_t i = 0; i < BUFFER_SZ; ++i) {
        for (std::size_t j = 0; j < BUFFER_SZ; ++j) {
            // We don't allow pairs of the same item
            if (i == j) {continue;}
            ++sum_map[buffer[i] + buffer[j]];
        }
    }
}


/**
 * Finds the first occurance of a number which doesn't follow the 25 sum rule
 * @param lines Vector of strings, each element is a line from stdin
 * @return The first number which doesn't follow the rule
 */
std::size_t solution1(std::vector<std::string> &lines) {
    std::array<long long int, BUFFER_SZ> buffer;
    std::unordered_map<std::size_t, int> sum_map;

    // Populate buffer
    for (std::size_t i = 0; i < BUFFER_SZ; ++i) {
        buffer[i] = std::stoll(lines[i]);
    }

    // Populate pair sum map
    set_sum_pair(sum_map, buffer);

    // Check for input which doesn't follow rule
    for (std::size_t i = BUFFER_SZ; i < lines.size(); ++i) {
        size_t num = std::stoll(lines[i]);
        // Found occurance which isn't a pair sum
        if (sum_map.find(num) == sum_map.end()) {
            return num;
        }

        // Otherwise add to buffer and reorganize pair sum
        buffer[i % BUFFER_SZ] = num;
        set_sum_pair(sum_map, buffer);

    }

    throw "All input matches rule";
}


/**
 * Finds the sum of smallest/largest of contiguous set which sums to target
 * @param lines Vector of strings, each element is a line from stdin
 * @param target The target sum to find
 * @return The sum of smallest + largest in list summing to target
 */
std::size_t solution2(std::vector<std::string> &lines, std::size_t target) {
    for (std::size_t start = 0; start < lines.size() - 1; ++start) {
        std::size_t end = start + 1;
        std::size_t sum = std::stoll(lines[start]);

        // Continue until we find sum or reach end of input
        while (sum < target && end < lines.size()) {
            sum += std::stoll(lines[end]);
            if (sum == target) {
                return std::stoll(lines[start]) + std::stoll(lines[end]);
            }
            ++end;
        }
    }
    throw "Could not find sequence summing to target";
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    std::size_t num1 = solution1(lines);
    std::cout << "First occurance not matching rule in part 1: " << num1 << std::endl;
    std::size_t num2 = solution2(lines, num1);
    std::cout << "Sum of first/last in part 2: " << num2 << std::endl;
}