#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <unordered_map>
#include <algorithm>         // find
#include <regex>
#include <cassert>

#include "common.h"


// consts
const std::vector<std::string> REQUIRED_TOKES = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
typedef bool (*CheckFunction)(std::string);


/**
 * Gets a map of passport data filed by name
 * @param lines Vector of strings, each element is a line from stdin
 * @param i Current index in the lines vector
 * @return Map of passport data fields
 */
std::unordered_map<std::string, std::string> get_passport_map(std::vector<std::string> &lines, size_t &i) {
    std::unordered_map<std::string, std::string> passport;

    // Get passport data
    while (lines[i] != "") {
        // Split string by space
        std::istringstream iss(lines[i]);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

        for (const auto & item : tokens) {
            size_t idx = item.find(':');
            assert (idx != std::string::npos);
            passport[item.substr(0, idx)] = item.substr(idx + 1);
        }
        ++i;
    }

    return passport;
}

// --------------------------------
// | Helper valid check functions |
// --------------------------------

bool byr_valid(std::string byr) {
    // Year falls in [1920, 2002]
    int year = std::stoi(byr);
    return (year >= 1920 && year <= 2002);
}

bool iyr_valid(std::string iyr) {
    // Year falls in [2010, 2020]
    int year = std::stoi(iyr);
    return (year >= 2010 && year <= 2020);
}

bool eyr_valid(std::string eyr) {
    // Year falls in [2020, 2030]
    int year = std::stoi(eyr);
    return (year >= 2020 && year <= 2030);
}

bool hgt_valid(std::string hgt) {
    // Height in range depending on cm vs in
    assert (hgt.size() >= 3);
    std::string units = hgt.substr(hgt.size() - 2);
    int height = std::stoi(hgt.substr(0, hgt.size() - 2));    
    return (units == "in" && (height >= 59 && height <= 76)) || (units == "cm" && (height >= 150 && height <= 193));
}

bool hcl_valid(std::string hcl) {
    // Hair colour is a 6 digit hex value preceeded by #
    static const std::string valid_hcl = "0123456789abcdef";
    if (hcl.size() != 7 || hcl[0] != '#') {
        return false;
    }
    for (int i = 1; i < hcl.size(); ++i) {
        if (valid_hcl.find(hcl[i]) == std::string::npos) {
            return false;
        }
    }
    return true;
}

bool ecl_valid(std::string ecl) {
    // eye colour is from a predetermined list
    static const std::vector<std::string> valid_ecl = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    return std::find(valid_ecl.begin(), valid_ecl.end(), ecl) != valid_ecl.end();
}

bool pid_valid(std::string pid) {
    // passport id is a 9 digit number
    static const std::regex pattern("[0-9]+");
    return pid.size() == 9 && std::regex_match(pid, pattern);
}


/**
 * Counts the number of valid passports, only checks if a data field is present.
 * @param lines Vector of strings, each element is a line from stdin
 * @return Count of valid passports
 */
int solution1(std::vector<std::string> &lines) {
    int count = 0;

    for (std::size_t i = 0; i < lines.size(); ++i) {
        // Get passport data map
        std::unordered_map<std::string, std::string> passport = get_passport_map(lines, i);

        // Check if passport valid
        bool valid = true;
        for (const auto & token : REQUIRED_TOKES) {
            if (passport.find(token) == passport.end()) {
                valid = false;
                break;
            }
        }
        if (valid) { ++count; }
    }

    return count;
}


/**
 * Counts the number of valid passports, which follow more strict rules
 * @param lines Vector of strings, each element is a line from stdin
 * @return Count of valid passports
 */
int solution2(std::vector<std::string> &lines) {
    int count = 0;
    // Function map to help for easy access to data field validity checks
    static const std::unordered_map<std::string, CheckFunction> func_map {
        {"byr", &byr_valid},
        {"iyr", &iyr_valid},
        {"eyr", &eyr_valid},
        {"hgt", &hgt_valid},
        {"hcl", &hcl_valid},
        {"ecl", &ecl_valid},
        {"pid", &pid_valid}
    };

    for (std::size_t i = 0; i < lines.size(); ++i) {
        // Get passport data map
        std::unordered_map<std::string, std::string> passport = get_passport_map(lines, i);

        // Check if passport valid
        bool valid = true;
        for (const auto & token : REQUIRED_TOKES) {
            // data field present and valid
            if (passport.find(token) == passport.end() || !func_map.at(token)(passport[token])) {
                valid = false;
                break;
            }

        }
        if (valid) { ++count; }
    }

    return count;
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    int count1 = solution1(lines);
    std::cout << "Number of valid passports in part 1: " << count1 << std::endl;
    long long int count2 = solution2(lines);
    std::cout << "Number of valid passports in part 2: " << count2 << std::endl;
}