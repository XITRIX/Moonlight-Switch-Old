#pragma once
#include <string>
#include <streaming/GameStreamClient.hpp>

void connectAndPair(std::string address, std::function<void(void)> connectResult);