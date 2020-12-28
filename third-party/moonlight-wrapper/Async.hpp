#include <vector>
#include <mutex>
#include <algorithm>
#pragma once

class Async {
public:
    static void run(std::function<void()> task);
    static void stop();
};