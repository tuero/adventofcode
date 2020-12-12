#include <iostream>
#include <stdlib.h>     // exit
#include <vector>


namespace common {

template <typename T>
std::vector<T> read_stdin() {
    std::vector<T> inputs;
    T input;

    while (std::cin >> input) {
        inputs.push_back(input);
    }

    if (!std::cin.eof()) {
        std::cerr << "Error proccessing input." << std::endl;
        exit(1);
    }

    return inputs;
}

} // namespace common

