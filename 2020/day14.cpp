#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>            // replace, count
#include <cstdint>              // types
#include <cassert>

#include "common.h"


// consts
const std::string default_mask = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";


/**
 * Set a bit for the given value
 * @param value The value to set a bit
 * @param idx The index to shift to set the bit
 * @param bit The bit value to use
 * @return The value with the bit set to the given bit
 */
uint64_t set_bit(uint64_t value, int idx, uint64_t bit) {
    value ^= (-bit ^ value) & (1ULL << idx);
    return value;
}


/**
 * Mask a given value using the X/1/0 key
 * @param value The value
 * @param mask The string mask
 * @return The value with the mask applied
 */
uint64_t mask_value(uint64_t value, const std::string mask) {
    int shift = 0;
    for (int i = mask.size() - 1; i >= 0; --i) {
        if (mask[i] != 'X') {
            uint64_t bit = (mask[i] == '0') ? 0 : 1;
            value = set_bit(value, shift, bit);
        }
        ++shift;
    }
    return value;
}


/**
 * Gets the program memory sum
 * @param lines Vector of strings, each element is a line from stdin
 * @return Sum of values remaining in memory
 */
uint64_t solution1(const std::vector<std::string> &lines) {
    std::string mask = default_mask;
    std::unordered_map<uint64_t, uint64_t> memory;

    for (const auto & line : lines) {
        std::size_t val_idx = line.find(" = ") + 3;
        std::size_t mem_start = line.find("["), mem_end = line.find("]");
        
        if (mem_start != std::string::npos) {
            // Set memory value
            uint64_t addr = std::stoll(line.substr(mem_start + 1, mem_start - mem_end));
            uint64_t value = std::stoll(line.substr(val_idx));
            memory[addr] = mask_value(value, mask);
        }  else {
            // Set mask
            mask = line.substr(val_idx);
        }
    }

    // Find sum of memory
    uint64_t result = 0;
    for (const auto & m : memory) {
        result += m.second;
    }
    return result;
}


/**
 * Gets the program memory sum
 * @param lines Vector of strings, each element is a line from stdin
 * @return Sum of values remaining in memory
 */
uint64_t solution2(const std::vector<std::string> &lines) {
    std::string mask = default_mask;
    std::unordered_map<uint64_t, uint64_t> memory;

    for (const auto & line : lines) {
        std::size_t val_idx = line.find(" = ") + 3;
        std::size_t mem_start = line.find("["), mem_end = line.find("]");
        
        if (mem_start != std::string::npos) {
            // Set initial address
            uint64_t addr = std::stoull(line.substr(mem_start + 1, mem_start - mem_end));
            uint64_t value = std::stoull(line.substr(val_idx));
            std::string addr_mask = mask;
            std::replace(addr_mask.begin(), addr_mask.end(), '0', 'X');
            addr = mask_value(addr, addr_mask);

            // Loop for each 2^n possibilities of the float bits
            std::size_t num_x = std::count(mask.begin(), mask.end(), 'X');
            for (std::size_t bit_seq = 0; bit_seq < (1 << num_x); ++bit_seq) {
                uint64_t curr_addr = addr;
                uint64_t bit_idx = 0;
                for (int j = mask.size() - 1; j >= 0; --j) {
                    if (mask[j] == 'X') {
                        // addr_mask
                        unsigned long bit = (bit_seq >> bit_idx) & 1ULL;
                        curr_addr = set_bit(curr_addr, mask.size() - j - 1, bit);
                        assert (curr_addr > 0ULL);
                        ++bit_idx;
                    }
                }
                memory[curr_addr] = value;
            }
        } else {
            // Set mask
            mask = line.substr(val_idx);
        }
    }

    // Find sum of memory
    uint64_t result = 0;
    for (const auto & m : memory) {
        result += m.second;
    }
    return result;
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    std::size_t num1 = solution1(lines);
    std::cout << "Bus in part 1: " << num1 << std::endl;
    uint64_t num2 = solution2(lines);
    std::cout << "Bus in part 2: " << num2 << std::endl;
}