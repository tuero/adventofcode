#include <iostream>
#include <vector>
#include <algorithm>        // sort

#include "common.h"


// consts
const int SUM = 2020;
const int NO_SOLUTION = -1;


/**
 * Trivial solution, checks all pairwise elements
 * 
 * @param items Vector of numbers
 * @param sum_to_fund The sum for the pair to find
 * @return Product of number pair which matches sum
 */
int solution1_trivial(std::vector<int> &items, int sum_to_find) {
    // Trivial solution. Here, we simply check every possible pair
    for (std::size_t i = 0; i < items.size(); ++i) {
        for (std::size_t j = i + 1; j < items.size(); ++j) {
            if (items[i] + items[j] == sum_to_find) {
                return items[i] * items[j];
            }
        }
    }

    return NO_SOLUTION;
}

/**
 * Same as above, but sorts the list of ints first then 
 * finds pair by moving left/right points towards center
 * 
 * @param items Vector of numbers
 * @param sum_to_fund The sum for the pair to find
 * @return Product of number pair which matches sum
 */
int solution1(std::vector<int> &items, int sum_to_find) {
    std::sort(items.begin(), items.end());

    auto it_left = items.begin();
    auto it_right = std::prev(items.end());

    // Continue until iterators touch or we find solution
    while (it_left != it_right) {
        int left_val = *it_left, right_val = *it_right;
        int curr_sum = left_val + right_val;

        // Found sum
        if (curr_sum == sum_to_find) {
            return left_val * right_val;
        } 
        // Sum is too large, move right iterator down
        else if (curr_sum > sum_to_find) {
            it_right = std::prev(it_right);
        }
        // Sum is too small, move left iterator up
        else if (curr_sum < sum_to_find) {
            it_left = std::next(it_left);
        }
    }

    return NO_SOLUTION;
}


/**
 * Trivial solution, checks all triplet pairs of elements
 * 
 * @param items Vector of numbers
 * @param sum_to_fund The sum for the triplet to find
 * @return Product of number triplet which matches sum
 */
int solution2_trivial(std::vector<int> &items, int sum_to_find) {
    for (std::size_t i = 0; i < items.size(); ++i) {
        for (std::size_t j = i + 1; j < items.size(); ++j) {
            for (std::size_t k = j + 1; k < items.size(); ++k) {
                if (items[i] + items[j] + items[k] == sum_to_find) {
                    return items[i] * items[j] * items[k];
                }
            }
        }
    }

    return NO_SOLUTION;
}


/**
 * Same as solution1, but we first lock the first term of the triplet,
 * then use the method from solution 1
 * 
 * @param items Vector of numbers
 * @param sum_to_fund The sum for the triplet to find
 * @return Product of number triplet which matches sum
 */
int solution2(std::vector<int> &items, int sum_to_find) {
    std::sort(items.begin(), items.end());

    // Hold the fist item constant
    for (size_t i = 0; i < items.size(); ++i) {
        int starting_val =  items[i];
        size_t it_left = 0, it_right = items.size() - 1;

        // Continue until iterators touch or we find solution
        while (it_left != it_right) {
            int left_val = items[it_left], right_val = items[it_right];
            int curr_sum = left_val + right_val + starting_val;

            // Found sum
            if (curr_sum == sum_to_find) {
                return left_val * right_val * starting_val;
            } 
            // Sum is too large, move right iterator down
            else if (curr_sum > sum_to_find) {
                --it_right;
            }
            // Sum is too small, move left iterator up
            else if (curr_sum < sum_to_find) {
                ++it_left;
            }
        }
    }

    return NO_SOLUTION;
}

int main() {
    std::vector<int> items = common::read_stdin<int>();
    
    int result1 = solution1(items, SUM);
    if (result1 == NO_SOLUTION) {
        std::cout << "No solution found for part 1." << std::endl;
    } else {
        std::cout << "Solution for part 1 is " << result1 << std::endl; 
    }

    int result2 = solution2(items, SUM);
    if (result2 == NO_SOLUTION) {
        std::cout << "No solution found for part 2." << std::endl;
    } else {
        std::cout << "Solution for part 2 is " << result2 << std::endl; 
    }
}