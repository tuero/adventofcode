#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>            // std::find
#include <regex>
#include <cassert>

#include "common.h"


// consts
const std::string bag_to_find = "shiny gold";

// Bag struct object. Identified by bag string, but also stores the count
struct BagCount {
    std::string bag;
    mutable int count;

    BagCount(std::string bag, int count = 0) : bag(bag), count(count) {}

    bool operator==(const BagCount & other) const {
        return bag == other.bag;
    }
};

// We only care about unique bag hashes
struct BagHash {
    size_t operator()(const BagCount & bag_count) const {
        return std::hash<std::string>()(bag_count.bag);
    }
};

typedef std::unordered_set<BagCount, BagHash> BagSet;
typedef std::unordered_map<std::string, BagSet> BagMap;


/**
 * Generate the bag map of every bag each bag can hold
 * @param lines Vector of strings, each element is a line from stdin
 * @return Map of question counts
 */
BagMap generate_bag_map(std::vector<std::string> &lines) {
    BagMap bag_map;
    std::regex rgx("[0-9]+\\s[a-z]+\\s[a-z]+\\sbag");

    for (const auto & line : lines) {
        std::smatch match;
        std::string s = line;
        BagSet bags;
        std::string current_bag = line.substr(0, line.find(" bags contain"));
        
        // Get all bags mentioned
        while (std::regex_search (s, match, rgx)) {
            std::string bag_str = match[0].str();
            int count = stoi(bag_str.substr(0, bag_str.find(" ")));
            int start_idx = bag_str.find(" ") + 1;
            std::string bag = bag_str.substr(start_idx, bag_str.size() - 4 - start_idx);
            bags.insert({bag, count});
            s = match.suffix().str();
        }

        // Store top-level mappings
        for (const auto & b : bags) {
            bag_map[current_bag].insert(b);
        }

    }

    // Consolidate the bag groupings
    // If an inner bag contains the outer, add all the outer bag's children to the inner bag
    for (auto outer_bag : bag_map) {
        for (auto & inner_bag : bag_map) {
            if (inner_bag.first == outer_bag.first) {continue;}
            // Inner bag contains outer bag
            auto itr = inner_bag.second.find((BagCount){outer_bag.first, 1});
            if (itr != inner_bag.second.end()) {
                for (const auto & b : outer_bag.second) {
                    // If new bag than insert, otherwise we add to existing coun
                    BagSet::iterator itr2 = inner_bag.second.find((BagCount){b.bag, 1});
                    if (itr2 == inner_bag.second.end()) {
                        inner_bag.second.insert({b.bag, b.count * itr->count});
                    }
                    else {
                        itr2->count += b.count * itr->count;
                    }
                }
            }
        }
    }

    return bag_map;
}


/**
 * Gets the number of bags which can eventually hold a gold bag
 * @param lines Vector of strings, each element is a line from stdin
 * @return Number of bags
 */
long long int solution1(std::vector<std::string> &lines) {
    long long int count = 0;
    BagMap bag_map = generate_bag_map(lines);

    // Count number of bags which contain shiny gold bag
    for (const auto & bag : bag_map) {
        if (bag.second.find({bag_to_find, 0}) != bag.second.end()) {
            ++count;
        }
    }

    return count;
}


/**
 * Gets the number of bags inside the gold shiny bag
 * @param lines Vector of strings, each element is a line from stdin
 * @return Number of bags
 */
long long int solution2(std::vector<std::string> &lines) {
    long long int count = 0;
    BagMap bag_map = generate_bag_map(lines);

    // Count number of bags which contain shiny gold bag
    for (const auto & bag_count : bag_map[bag_to_find]) {
        std::cout << bag_count.bag << " " << bag_count.count << std::endl;
        count += bag_count.count;
    }

    return count;
}


int main() {
    // Get data from stdin
    std::vector<std::string> lines = common::read_stdin_lines();

    int count1 = solution1(lines);
    std::cout << "Sum of bags in part 1: " << count1 << std::endl;
    int count2 = solution2(lines);
    std::cout << "Sum of bags in part 2: " << count2 << std::endl;
}