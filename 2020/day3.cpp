#include <iostream>
#include <vector>
#include <array>
#include <cassert>

#include "common.h"


/**
 * Given a path (represented by the X/Y offsets), counts the number of trees
 * passed along the traveled path.
 * 
 * @param lines Vector of strings, each element is a line from stdin
 * @param dx The displacement in X along which the path travels
 * @param dy The displacement in Y along which the path travels
 * @return Count of trees passed along the path
 */
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


/**
 * Just counts the trees along a single path.
 * 
 * @param lines Vector of strings, each element is a line from stdin
 * @return Count of trees passed along the path
 */
int solution1(std::vector<std::string> &lines) {
    return count_trees(lines, 3, 1);
}


/**
 * Counts the trees along a list of paths and multiples the counts
 * 
 * @note Possible overflow, uses 64bit ints.
 * 
 * @param lines Vector of strings, each element is a line from stdin
 * @return Count of trees passed along the path
 */
long long int solution2(std::vector<std::string> &lines) {
    static const std::vector<std::array<int, 2>> offsets = 
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
    std::cout << "Number of trees along path for part 2: " << count2 << std::endl;
}