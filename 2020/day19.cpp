#include <iostream>
#include <vector>
#include <deque>
#include <unordered_set>
#include <string>
#include <unordered_map>
#include <regex>
#include <algorithm>            // find
#include <cstdint>              // types
#include <cassert>

#include "common.h"


// define and consts
const std::unordered_set<std::string> NON_EXPR = {"|", "(", ")+"};
typedef std::unordered_map<std::string, std::string> RuleMap;


/**
 * Get the expression rules
 * @param lines Vector of strings containing rules
 * @return Map of rule ID and rule expression
 */
RuleMap get_rules(const std::vector<std::string> &lines) {
    RuleMap r_map;
    for (const auto & line : lines) {
        std::size_t idx = line.find(':');
        r_map[line.substr(0, idx)] = line.substr(idx + 2);
    }
    return r_map;
}


/**
 * Create the regex pattern from the input rules
 * @param rule_map The mapping of rules + idx
 * @return The regex pattern which accepts all strings valid by the rules
 */
std::regex create_regex(const RuleMap & rule_map) {
    std::string pattern = " " + rule_map.at("0") + " ";
    std::vector<std::string> rules = common::split(rule_map.at("0"));
    std::deque<std::string> queue(rules.begin(), rules.end());

    // lambdas
    auto pop_and_return = [&]() {
        std::string value = queue.front();
        queue.pop_front();
        return value;
    };
    auto insert_non_expr = [](std::string val) {
        return NON_EXPR.find(val) == NON_EXPR.end();
    };

    // Exhaust all rules
    while (!queue.empty()) {
        const std::string & idx = pop_and_return();
        const std::string & rule = rule_map.at(idx); 
        
        // If rule is not char literal, we need to add child rules
        if (rule.find('"') == std::string::npos) {
            // Prevent doing a child rule multiple times
            std::unordered_set<std::string> to_add;
            for (const auto & child_rule : common::split(rule)) {
                if (to_add.find(child_rule) == to_add.end() && NON_EXPR.find(child_rule) == NON_EXPR.end()) {
                    queue.push_back(child_rule);
                }
            }
        }

        // Replacement for any occurance of the idx is our rule
        // Need to surround | with brackets to keep order of operations
        std::string replacement = (rule.find('|') == std::string::npos) ? rule : "( " + rule + " )";
        pattern = common::replace(pattern, " " + idx + " ", " " + replacement + " ");
    }

    // Cleanup by removing quotes around char literals
    common::erase(pattern, '"');
    common::erase(pattern, ' ');

    return std::regex(pattern);
}


/**
 * Gets the number of expression matching rule
 * @param rule_map The map of expression rules
 * @param input Vector of strings, each element is an input expression
 * @return Number of matched expressions
 */
int solution1(RuleMap rule_map, std::vector<std::string> input) {
    // Create regex for the rules
    std::regex r = create_regex(rule_map);

    // Check if inputs match rule
    int num_matched = 0;
    for (const auto & s : input) {
        if (std::regex_match(s, r)) {
            ++num_matched;
        }
    }
    return num_matched;
}


/**
 * Gets the number of expression matching rule
 * @param rule_map The map of expression rules
 * @param input Vector of strings, each element is an input expression
 * @return Number of matched expressions
 */
int solution2(RuleMap rule_map, std::vector<std::string> input) {
    // Need to update rules for part 2
    rule_map["8"] = "( 42 )+";
    rule_map["11"] = "42 31 | 42 42 31 31 | 42 42 42 31 31 31 | 42 42 42 42 31 31 31 31 | 42 42 42 42 42 31 31 31 31 31";

    // Create regex for the rules
    std::regex r = create_regex(rule_map);

    // Check if inputs match rule
    int num_matched = 0;
    for (const auto & s : input) {
        if (std::regex_match(s, r)) {
            ++num_matched;
        }
    }
    return num_matched;
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    // Get input + rules
    auto split_idx = std::find(lines.begin(), lines.end(), "");
    RuleMap rule_map = get_rules(std::vector<std::string>(lines.begin(), split_idx));
    std::vector<std::string> input(split_idx + 1, lines.end());

    // int num1 = solution1(rule_map, input);
    // std::cout << "Messages matching in part 1: " << num1 << std::endl;
    int num2 = solution2(rule_map, input);
    std::cout << "Messages matching in part 2: " << num2 << std::endl;
}