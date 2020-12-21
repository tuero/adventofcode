#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <cassert>
#include <cstdlib>

#include "common.h"


// consts
enum DIR {NORTH, EAST, SOUTH, WEST, DIR_MAX, LEFT, RIGHT, FORWARD};
const std::unordered_map<char, DIR> str_dir_map = {
    {'N', DIR::NORTH},
    {'E', DIR::EAST},
    {'S', DIR::SOUTH},
    {'W', DIR::WEST},
    {'L', DIR::LEFT},
    {'R', DIR::RIGHT},
    {'F', DIR::FORWARD},
};
const std::unordered_map<DIR, std::pair<int, int>> dir_offset_map = {
    {DIR::NORTH, {-1, 0}},
    {DIR::EAST, {0, 1}},
    {DIR::SOUTH, {1, 0}},
    {DIR::WEST, {0, -1}}
};


/**
 * Wrap around modulo
 */
int positive_modulo(int i, int n) {
    return (i % n + n) % n;
}


struct Ship {
    std::pair<int, int> coords;
    std::pair<int, int> waypoint;
    DIR cur_dir;

    // Ship starts at origin facing east
    Ship (std::pair<int, int> start = {0, 1}) : coords({0, 0}), waypoint(start), cur_dir(DIR::EAST) {}

    // Rotate ship by given direction + angle
    void rotate(DIR dir, int angle) {
        assert (dir == DIR::LEFT || dir == DIR::RIGHT);
        int multiplier = (dir == DIR::RIGHT) ? 1 : -1;
        while (angle > 0) {
            waypoint = {multiplier * waypoint.second, -multiplier * waypoint.first};
            angle -= 90;
        }
    }

    // Move the waypoint along the given direction
    void move_waypoint(DIR dir, int distance) {
        assert (dir < DIR::DIR_MAX);
        std::pair<int, int> displacement = dir_offset_map.at(dir);
        waypoint.first += displacement.first * distance;
        waypoint.second += displacement.second * distance;
    }

    // Move in waypoint direction
    void move(int distance) {
        coords.first += distance * waypoint.first;
        coords.second += distance * waypoint.second;
    }

    // Move in direction (disregarding the waypoint)
    void move(DIR dir, int distance) {
        std::pair<int, int> displacement = dir_offset_map.at(dir);
        coords.first += distance * displacement.first;
        coords.second += distance * displacement.second;
    }
};



/**
 * Moves the ship and find manhattan distance travelled
 * @param lines Vector of strings, each element is a line from stdin
 * @return The manhattan distance
 */
std::size_t solution1(const std::vector<std::string> &lines) {
    Ship ship;

    // Move ship
    for (const auto & line : lines) {
        DIR dir = str_dir_map.at(line[0]);
        int distance = std::stoi(line.substr(1));

        if (dir == DIR::LEFT || dir == DIR::RIGHT) {
            ship.rotate(dir, distance);
        } else if (dir < DIR::DIR_MAX) {
            ship.move(dir, distance);
        } else {
            ship.move(distance);
        }
    }
    
    // Manhattan distance
    return std::abs(ship.coords.first) + std::abs(ship.coords.second);
}


/**
 * Moves the ship and find manhattan distance travelled
 * @param lines Vector of strings, each element is a line from stdin
 * @return The manhattan distance
 */
std::size_t solution2(const std::vector<std::string> &lines) {
    Ship ship({-1, 10});

    // Move ship
    for (const auto & line : lines) {
        DIR dir = str_dir_map.at(line[0]);
        int distance = std::stoi(line.substr(1));

        if (dir == DIR::LEFT || dir == DIR::RIGHT) {
            ship.rotate(dir, distance);
        } else if (dir < DIR::DIR_MAX) {
            ship.move_waypoint(dir, distance);
        } else {
            ship.move(distance);
        }
    }
    
    // Manhattan distance
    return std::abs(ship.coords.first) + std::abs(ship.coords.second);
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    std::size_t num1 = solution1(lines);
    std::cout << "Manhattan distance in part 1: " << num1 << std::endl;
    std::size_t num2 = solution2(lines);
    std::cout << "Manhattan distance in part 2: " << num2 << std::endl;
}