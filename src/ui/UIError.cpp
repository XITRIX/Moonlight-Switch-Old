#include "UIError.hpp"

#include <borealis.hpp>

void showError(std::string message, std::function<void(void)> closeAction) {
    auto alert = new brls::Dialog(message);
    alert->addButton("Close", [alert, closeAction](auto view) 
    { 
        alert->close(); 
        if (closeAction != NULL) 
            closeAction(); 
    });
    alert->setCancelable(true);
    alert->open();
}