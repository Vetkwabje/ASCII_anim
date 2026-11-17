#pragma once

#include <utility>

class Terminal {
public:
    Terminal();
    ~Terminal();

    int readKey();

    std::pair<int, int> getSize() const;
};