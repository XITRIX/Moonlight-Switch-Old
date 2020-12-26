#pragma once
#include <string>
#include <streaming/GameStreamClient.hpp>

namespace ui {
    void connect(std::string address, std::function<void(void)> connectResult);
    void connectAndPair(std::string address, ServerCallback<Host> connectResult);
    void apps(Host host);
}