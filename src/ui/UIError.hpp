#pragma once

#include <string>
#include <functional>

void showError(std::string message, std::function<void(void)> closeAction = NULL);