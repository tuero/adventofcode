#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>

#include "common.h"


// consts
const int EMPTY = 0;
const int OCCUPIED = 1;
const int FLOOR = 2;
std::unordered_map<char, int> str_seat_map = {
    {'L', EMPTY},
    {'#', OCCUPIED},
    {'.', FLOOR}
};
std::unordered_map<int, char> seat_str_map = {
    {EMPTY, 'L'},
    {OCCUPIED, '#'},
    {FLOOR, '.'}
};

const std::array<std::pair<int, int>, 8> offsets = {{
    {-1, 0},
    {-1, 1},
    {0, 1},
    {1, 1},
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, -1}
}};

struct Grid {
    int rows, cols;
    std::vector<std::vector<int>> grid;

    Grid (const std::vector<std::string> &lines) {
        for (const auto & line : lines) {
            std::vector<int> row;
            for (const auto & c : line) {
                row.push_back(str_seat_map.at(c));
            }
            cols = row.size();
            grid.push_back(row);
        }
        rows = grid.size();
    }

    void print() const {
        for (std::size_t row = 0; row < rows; ++row) {
            for (std::size_t col = 0; col < cols; ++col) {
                std::cout << seat_str_map.at(grid[row][col]);
            }
            std::cout << std::endl;
        }
    }

    bool out_of_bounds(int row, int col) const {
        return (row < 0 || col < 0 || row >= rows || col >= cols);
    }

    int get_occupied_count() const {
        int num_occupied = 0;
        for (std::size_t row = 0; row < rows; ++row) {
            for (std::size_t col = 0; col < cols; ++col) {
                if (grid[row][col] == OCCUPIED) {++num_occupied;}
            }
        }
        return num_occupied;
    }
};


/**
 * Runs one simulation using simple rules
 * @param grid The grid struct of seats
 * @return True if there has been a change in the seats
 */
bool step_simulation1(Grid &grid) {
    bool has_changed = false;
    Grid prev = grid;

    // Simulate
    for (std::size_t row = 0; row < grid.rows; ++row) {
        for (std::size_t col = 0; col < grid.cols; ++col) {
            // Rule 3: Skip if floor
            if (prev.grid[row][col] == FLOOR) {continue;}
            int occupied_counter = 0;
            // Check number of occupied seats
            for (const auto & offset : offsets) {
                int row_n = row + offset.first, col_n = col + offset.second;
                if (prev.out_of_bounds(row_n, col_n)) {continue;}
                if (prev.grid[row_n][col_n] == OCCUPIED) {++occupied_counter;}
            }

            // Rule 1: Seat empty and no occupied seats adjacent
            if (prev.grid[row][col] == EMPTY && occupied_counter == 0) {
                grid.grid[row][col] = OCCUPIED;
                has_changed = true;
            }
            // Rule 2: Seat occupied and 4+ seats adjacent are occupied
            if (prev.grid[row][col] == OCCUPIED && occupied_counter >= 4) {
                grid.grid[row][col] = EMPTY;
                has_changed = true;
            }
        }
    }

    return has_changed;
}


/**
 * Runs one simulation using complex rules
 * @param grid The grid struct of seats
 * @return True if there has been a change in the seats
 */
bool step_simulation2(Grid &grid) {
    bool has_changed = false;
    Grid prev = grid;

    // Simulate
    for (std::size_t row = 0; row < grid.rows; ++row) {
        for (std::size_t col = 0; col < grid.cols; ++col) {
            // Rule 3: Skip if floor
            if (prev.grid[row][col] == FLOOR) {continue;}
            int occupied_counter = 0;
            // Check number of occupied seats
            for (const auto & offset : offsets) {
                int row_n = row + offset.first, col_n = col + offset.second;
                // Scan down the direction
                while (!prev.out_of_bounds(row_n, col_n)) {
                    if (prev.grid[row_n][col_n] != FLOOR) {
                        occupied_counter += (prev.grid[row_n][col_n] == OCCUPIED) ? 1 : 0;
                        break;
                    }
                    row_n += offset.first;
                    col_n += offset.second;
                }
            }

            // Rule 1: Seat empty and no occupied seats adjacent
            if (prev.grid[row][col] == EMPTY && occupied_counter == 0) {
                grid.grid[row][col] = OCCUPIED;
                has_changed = true;
            }
            // Rule 2: Seat occupied and 5+ seats adjacent are occupied
            if (prev.grid[row][col] == OCCUPIED && occupied_counter >= 5) {
                grid.grid[row][col] = EMPTY;
                has_changed = true;
            }
        }
    }

    return has_changed;
}


/**
 * Finds the number of simulations until a steady state is reached
 * @param lines Vector of strings, each element is a line from stdin
 * @return The number of simulations
 */
std::size_t solution1(const std::vector<std::string> &lines) {
    Grid grid(lines);
    while (step_simulation1(grid)) {}
    return grid.get_occupied_count();
}


/**
 * Finds the number of simulations until a steady state is reached
 * @param lines Vector of strings, each element is a line from stdin
 * @return The number of simulations
 */
std::size_t solution2(const std::vector<std::string> &lines) {
    Grid grid(lines);
    while (step_simulation2(grid)) {}
    return grid.get_occupied_count();
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    std::size_t num1 = solution1(lines);
    std::cout << "Number of seats occupied at equilibrium in part 1: " << num1 << std::endl;
    std::size_t num2 = solution2(lines);
    std::cout << "Number of seats occupied at equilibrium in part 2: " << num2 << std::endl;
}