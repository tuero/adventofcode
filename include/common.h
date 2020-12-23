#include <iostream>
#include <stdlib.h>     // exit
#include <vector>


namespace common {

template <typename T>
std::vector<T> read_stdin(const char delimiter = ' ') {
    std::vector<T> inputs;
    T input;

    while (std::cin >> input) {
        inputs.push_back(input);
        if (std::cin.peek() == delimiter) {
            std::cin.ignore();
        }
    }

    if (!std::cin.eof()) {
        std::cerr << "Error proccessing input." << std::endl;
        exit(1);
    }

    return inputs;
}

std::vector<std::string> read_stdin_lines() {
    std::vector<std::string> inputs;
    for (std::string line; std::getline(std::cin, line);) {
        inputs.push_back(line);
    }

    return inputs;
}

} // namespace common

