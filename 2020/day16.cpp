#include <iostream>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <sstream>
#include <regex>           
#include <cstdint>              // types
#include <cassert>

#include "common.h"


// consts
typedef std::array<int, 2> RuleRange;
struct Rule {
    std::string rule_name;
    RuleRange range1, range2;

    Rule (const std::string & name, RuleRange r1, RuleRange r2) 
        : rule_name(name), range1(r1), range2(r2) 
    {}

    bool in_range(int num) const {
        return (num >= range1[0] && num <= range1[1]) || (num >= range2[0] && num <= range2[1]);
    }
};


/**
 * Get the rule struct from the string input
 * @param line The string line
 * @return The rule struct
 */
Rule get_rule(const std::string & line) {
    std::string rule_name = line.substr(0, line.find(":"));
    std::vector<int> nums;

    std::smatch match;
    std::string s = line;
    std::regex rgx("[0-9]+");
    
    // Get all ranges
    while (std::regex_search (s, match, rgx)) {
        nums.push_back(std::stoll(match[0].str()));
        s = match.suffix().str();
    }
    assert (nums.size() == 4);
    return {rule_name, {nums[0], nums[1]}, {nums[2], nums[3]}};
}


/**
 * Get the numbers from the ticket
 * @param line The string ticket
 * @return A vector of ints representing the ticket
 */
std::vector<int> get_ticket_nums(const std::string & line) {
    std::vector<int> nums;
    std::stringstream ss(line);

    for (int i; ss >> i;) {
        nums.push_back(i);    
        if (ss.peek() == ',') {ss.ignore();}
    }

    return nums;
}

/**
 * Checks if the given ticket number is valid for ANY rule
 * @param rules The vector of rules
 * @param num The ticket number to check
 * @return True if the ticket is valid
 */
bool is_num_valid(std::vector<Rule> rules, int num) {
    for (const auto & rule : rules) {
        if (rule.in_range(num)) {
            return true;
        }
    }
    return false;
}


/**
 * Gets the sum of invalid ticker numbers
 * @param lines Vector of strings, each element is a line from stdin
 * @return Sum of values remaining in memory
 */
uint64_t solution1(const std::vector<std::string> &lines) {
    std::vector<Rule> rules;
    std::size_t i = 0;

    // Get rules
    while (lines[i] != "") {
        const std::string & line = lines[i++];
        rules.push_back(get_rule(line));
    }

    // Bring forward to nearby tickets section
    i += 5;
    
    // Check nearby tickets
    uint64_t invalid_sum = 0;
    while (i < lines.size()) {
        const std::string & line = lines[i++];
        for (const auto & num : get_ticket_nums(line)) {
            // Check if number falls into known range
            if (!is_num_valid(rules, num)) {
                invalid_sum += num;
                continue;
            }
        }
    }

    return invalid_sum;
}


/**
 * Gets the sum of invalid ticker numbers
 * @param lines Vector of strings, each element is a line from stdin
 * @return Sum of values remaining in memory
 */
uint64_t solution2(const std::vector<std::string> &lines) {
    std::vector<Rule> rules;
    std::size_t i = 0;

    // Get rules
    while (lines[i] != "") {
        const std::string & line = lines[i++];
        rules.push_back(get_rule(line));
    }

    // Bring forward to nearby tickets section and add our own ticket
    std::vector<std::vector<int>> valid_ticket_nums = {get_ticket_nums(lines[i+2])};
    i += 5;
    
    // Keep valid nearby tickets
    while (i < lines.size()) {
        bool valid = true;
        std::vector<int> nums = get_ticket_nums(lines[i++]);
        for (const auto & num : nums) {
            // Check if number falls into known range
            if (!is_num_valid(rules, num)) {
                valid = false;
                break;
            }
        }
        // Add if valid
        if (valid) { valid_ticket_nums.push_back(nums); }
    }

    // Initialize all rules as being possible for each index
    std::vector<std::unordered_set<int>> possibilities;
    for (const auto r : rules) {
        std::unordered_set<int> options;
        for (int k = 0; k < rules.size(); ++k) { options.insert(k); }
        possibilities.push_back(options);
    }

    // Start to find rules which do not work
    for (const auto & nums : valid_ticket_nums) {
        for (int j = 0; j < nums.size(); ++j) {
            for (std::size_t k = 0; k < rules.size(); ++k) {
                // Rule not satisfied
                if (!rules[k].in_range(nums[j])) { possibilities[k].erase(j); }
            }
        }
    }

    // Start to find solution
    std::unordered_map<int, int> mapping;
    for (int j = 0; j < rules.size(); ++j) {
        for (int k = 0; k < possibilities.size(); ++k) {
            // Look for possibilities with only 1 option
            if (possibilities[k].size() == 1) {
                int idx = *possibilities[k].begin();
                mapping[k] = idx;

                // Remove mapping from rest of rules and continue
                for (auto & p : possibilities) { p.erase(idx); }
                break;
            }
        }
    }

    uint64_t invalid_prod = 1;
    std::vector<int> my_ticket = valid_ticket_nums[0];
    for (int j = 0; j < rules.size(); ++j) {
        // Check for rule name start
        if (rules[j].rule_name.substr(0, 9) == "departure") {
            invalid_prod *= my_ticket[mapping[j]];
        }
    }

    return invalid_prod;
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    uint64_t num1 = solution1(lines);
    std::cout << "Error rate in part 1: " << num1 << std::endl;
    uint64_t num2 = solution2(lines);
    std::cout << "Error rate in part 2: " << num2 << std::endl;
}