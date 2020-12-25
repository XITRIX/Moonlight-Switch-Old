#include "UIError.hpp"

#include <borealis.hpp>

void showError(std::string message) {
    auto alert = new brls::Dialog(message);
    alert->addButton("Close", [alert](auto view) { alert->close(); });
    alert->setCancelable(true);
    alert->open();
}