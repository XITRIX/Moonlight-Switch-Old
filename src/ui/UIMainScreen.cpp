#include "UIMainScreen.hpp"

#include <borealis.hpp>
#include <UIError.hpp>

void connectAndPair(std::string address, std::function<void(void)> connectResult)
{
    GameStreamClient::client()->connect(address, [address, connectResult](GSResult<SERVER_DATA> result) {
        if (result.isSuccess())
        {
            if (result.value().paired) { return connectResult(); }

            // Pair
            char r_pin[5];
            sprintf(r_pin, "%d%d%d%d",
                (int)rand() % 10, (int)rand() % 10, (int)rand() % 10, (int)rand() % 10);
            auto pin        = std::string(r_pin);
            auto pair_alert = new brls::Dialog("Please enter the following PIN\non the target PC:\n\n" + pin);
            // pair_alert->cancelable = false;
            pair_alert->open();
            GameStreamClient::client()->pair(address, pin, [pair_alert, connectResult](auto result) {
                pair_alert->close();

                if (result.isSuccess())
                {
                    brls::Application::notify("Success");
                    return connectResult();
                }
                else
                {
                    showError(result.error());
                    return connectResult();
                }
            });
        }
        else
        {
            showError(result.error());
        }
    });
}