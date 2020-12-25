#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <string>
#include <cstdint>              // types
#include <cassert>

#include "common.h"


// consts
typedef std::array<int, 3> Point;
const int NUM_SIMS = 6;
const char ACTIVE = '#';
const char INACTIVE = '.';


/**
 * Simple power function, good enough for low base/exponents
 * @param base power base
 * @param exp power exponent
 * @return The base^power
 */
uint64_t power(uint64_t base, uint64_t exp) {
    uint64_t result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

/**
 * Boosts hash combine
 */
struct ArrayHasher {
    std::size_t operator()(const std::vector<int> & a) const {
        std::size_t h = 0;
        for (auto e : a) {
            h ^= std::hash<int>{}(e) + 0x9e3779b9 + (h << 6) + (h >> 2); 
        }
        return h;
    }   
};

typedef std::unordered_map<std::vector<int>, int, ArrayHasher> CubeMap;

/**
 * Initialize the active cube points from input
 * @param lines Vector of strings, each element is a line from stdin
 * @param dim Number of dimensions
 * @return Starting active cubes
 */
CubeMap init_cubes(const std::vector<std::string> &lines, int dim) {
    CubeMap active_cubes;

    // Get starting cubes, set first item read as (0,0,0)
    for (int i = 0; i < lines.size(); ++i) {
        for (int j = 0; j < lines[i].size(); ++j) {
            if (lines[i][j] == ACTIVE) { 
                std::vector<int> point(dim, 0);
                point[0] = j;
                point[1] = i;
                ++active_cubes[point]; 
            }
        }
    }

    return active_cubes;
}


/**
 * Generate all neighbours for a given point
 * @param point The given point
 * @return Vector of points which are distance 1 away from given reference point
 */
std::vector<std::vector<int>> generate_neighbours(const std::vector<int> & point) {
    std::vector<std::vector<int>> neighbours;
    for (uint64_t i = 0; i < power(3, point.size()); ++i) {
        std::vector<int> candidate_point;
        int idx = i;
        for (int j = 0; j < point.size(); ++j) {
            candidate_point.push_back((idx % 3) + point[j] - 1);
            idx = idx / 3;
        }
        if (candidate_point != point) {
            neighbours.push_back(candidate_point);
        }
    }
    return neighbours;
}


/**
 * Run one simulation
 * @param active_cubes Current active cubes
 * @return Active cubes after simulation
 */
CubeMap simulate(CubeMap & active_cubes) {
    CubeMap next_cubes;
    CubeMap neighbour_counts;
    for (auto & cube : active_cubes) {
        int active_neighbours = 0;
        // Count active neighbours of active cube
        for (const auto & n : generate_neighbours(cube.first)) {
            ++neighbour_counts[n];
            if (active_cubes.find(n) != active_cubes.end()) {
                ++active_neighbours;
            }
        }

        // Cube stays active if 2 or 3 neighbours are active, otherwise stays active
        if (active_neighbours == 2 || active_neighbours == 3) {
            ++next_cubes[cube.first];
        }
    }

    // Inactive cube becomes active if 3 neighbours are active
    for (const auto & cube : neighbour_counts) {
        if (cube.second == 3) {
            ++next_cubes[cube.first];
        }
    }

    return next_cubes; 
}


/**
 * Gets the number of active cubes
 * @param lines Vector of strings, each element is a line from stdin
 * @return Number of active cubes
 */
uint64_t solution1(const std::vector<std::string> &lines) {
    // Get starting cubes, set first item read as (0,0,0, ...)
    CubeMap active_cubes = init_cubes(lines, 3);

    // Simulate
    for (int i = 0; i < NUM_SIMS; ++i) {
        active_cubes = simulate(active_cubes);
    }
    
    // Number of active cubes
    return active_cubes.size();
}


/**
 * Gets the number of active cubes
 * @param lines Vector of strings, each element is a line from stdin
 * @return Number of active cubes
 */
uint64_t solution2(const std::vector<std::string> &lines) {
    // Get starting cubes, set first item read as (0,0,0, ...)
    CubeMap active_cubes = init_cubes(lines, 4);

    // Simulate
    for (int i = 0; i < NUM_SIMS; ++i) {
        active_cubes = simulate(active_cubes);
    }
    
    // Number of active cubes
    return active_cubes.size();
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    uint64_t num1 = solution1(lines);
    std::cout << "Active cubes in part 1: " << num1 << std::endl;
    uint64_t num2 = solution2(lines);
    std::cout << "Active cubes in part 2: " << num2 << std::endl;
}