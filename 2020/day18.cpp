#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <unordered_map>
#include <variant>
#include <cstdint>              // types
#include <cassert>

#include "common.h"


// define and consts
typedef std::variant<int64_t, std::string> Op;
const Op LB = static_cast<Op>("(");
const Op RB = static_cast<Op>(")");


/**
 * Apply the operator to the given values
 * @param lhs First value
 * @param rhs Second value
 * @param op Operator to apply
 */
int64_t apply_operator(int64_t lhs, int64_t rhs, const std::string & op) {
    if (op == "+") {
        return lhs + rhs;
    } else if (op == "*") {
        return lhs * rhs;
    }
    assert (0);
    return -1;
}


/**
 * Convert the given string to infix operators
 * @param line The input string
 * @return Vector of ops in infix order
 */
std::vector<Op> str_to_infix(std::string line) {
    std::vector<Op> infix;
    std::string curr;
    for (const std::string & val : common::split(line)) {
        // Left/right bracket don't have space between them
        curr = val;
        int brack_counter = 0;
        while (true) {
            if (curr.front() == '(') {
                infix.push_back(LB);
                curr = curr.substr(1);
            } else if (curr.back() == ')') {
                ++brack_counter;
                curr = curr.substr(0, curr.size() - 1);
            } else {
                infix.push_back((common::is_number(curr) ? static_cast<Op>(std::stoll(curr)) : static_cast<Op>(curr)));
                break;
            }
        }
        // Need to fix the brackets
        for (int i = 0; i < brack_counter; ++i) {
            infix.push_back(RB);
        }
    }
    return infix;
}


/**
 * Convert the given infix order to postfix order
 * @param infix The input infix order
 * @param op_pres Operator precedence map
 * @return Vector of ops in postfix order
 */
std::vector<Op> infix_to_postfix(std::vector<Op> infix, std::unordered_map<std::string, int> op_pres) {
    std::vector<Op> postfix;
    std::stack<Op> st;
    auto pop_and_save = [&](){
        postfix.push_back(st.top());
        st.pop();
    };

    for (const auto & op : infix) {
        // Scanned op is an operand, add to output
        if (op.index() == 0) {
            postfix.push_back(op);
        } else if (op == LB) {
            st.push(op);
        } else if (op == RB) {
            // pop until we find matching bracket or 
            while (!st.empty() && st.top() != LB) {
                pop_and_save();
            }
            assert (!st.empty());
            st.pop();
        } else {
            // Scanned operator
            while (!st.empty() && op_pres[std::get<std::string>(op)] <= op_pres[std::get<std::string>(st.top())]) {
                pop_and_save();
            }
            st.push(op);
        }
    }

    // Remove any remaining ops
    while (!st.empty()) { pop_and_save(); }

    return postfix; 
}


/**
 * Evaluate the postfix expression
 * @param ops Vector of ops in postix notation
 * @return result of the expression
 */
int64_t posfix_eval(std::vector<Op> ops) {
    std::stack<int64_t> st;
    auto pop_and_return = [&](){
        int64_t val = st.top();
        st.pop();
        return val;
    };

    for (const auto op : ops) {
        // If operand then push value to stack
        if (op.index() == 0) {
            st.push(std::get<int64_t>(op));
        } else {
            // operator, get previous 2 element and apply 
            assert (st.size() >= 2);
            int64_t lhs = pop_and_return();
            int64_t rhs = pop_and_return();
            st.push(apply_operator(lhs, rhs, std::get<std::string>(op)));
        }
    }
    assert (st.size() == 1);
    return st.top();
}


/**
 * Gets the sum of resulting formula
 * @param lines Vector of strings, each element is a line from stdin
 * @return Sum of results
 */
int64_t solution1(const std::vector<std::string> &lines) {
    int64_t result = 0;
    std::unordered_map<std::string, int> op_pres = {
        {"+", 2},
        {"*", 2},
    };

    for (const auto & line : lines) {
        // Get list of ops
        std::vector<Op> infix = str_to_infix(line);

        // convert infix to postfix notation
        std::vector<Op> postfix = infix_to_postfix(infix, op_pres);

        // Calculate resut using postfix
        result += posfix_eval(postfix);
    }
    return result;
}


/**
 * Gets the sum of resulting formula
 * @param lines Vector of strings, each element is a line from stdin
 * @return Sum of results
 */
int64_t solution2(const std::vector<std::string> &lines) {
    int64_t result = 0;
    std::unordered_map<std::string, int> op_pres = {
        {"+", 2},
        {"*", 1},
    };

    for (const auto & line : lines) {
        // Get list of ops
        std::vector<Op> infix = str_to_infix(line);

        // convert infix to postfix notation
        std::vector<Op> postfix = infix_to_postfix(infix, op_pres);

        // Calculate resut using postfix
        result += posfix_eval(postfix);
    }
    return result;
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    int64_t num1 = solution1(lines);
    std::cout << "Sum of values in part 1: " << num1 << std::endl;
    int64_t num2 = solution2(lines);
    std::cout << "Sum of values in part 2: " << num2 << std::endl;
}