#include <iostream>
#include <vector>
#include <array>
#include <cassert>

#include "common.h"

int count_trees(std::vector<std::string> &lines, int dx, int dy) {
    assert (lines.size() > 0);

    int count = 0;
    std::size_t col = dx, row = dy;
    std::size_t height = lines.size(), width = lines[0].size();

    while (row < height) {
        // Tree, increment count
        if (lines[row][col] == '#') {
            ++count;
        }

        // Update path index
        col = (col + dx) % width;
        row += dy;
    }

    assert (count < height);
    return count;
}


int solution1(std::vector<std::string> &lines) {
    return count_trees(lines, 3, 1);
}

long long int solution2(std::vector<std::string> &lines) {
    std::vector<std::array<int, 2>> offsets = 
    {
        {1, 1},
        {3, 1},
        {5, 1},
        {7, 1},
        {1, 2}
    };
    
    long long int count = 1;
    for (auto const & offset : offsets) {
        count *= count_trees(lines, offset[0], offset[1]);
    }

    return count;
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    int count1 = solution1(lines);
    std::cout << "Number of trees along path for part 1: " << count1 << std::endl;
    long long int count2 = solution2(lines);
    std::cout << "Number of trees along path for part 1: " << count2 << std::endl;
}