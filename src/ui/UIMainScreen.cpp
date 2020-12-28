#include "UIMainScreen.hpp"

#include <AppListView.hpp>
#include <UIError.hpp>
#include <borealis.hpp>

void ui::connect(std::string address, std::function<void(void)> connectResult) 
{
    ui::connectAndPair(address, [address, connectResult](auto result) {
        if (result.isSuccess()) {
            connectResult();
            ui::apps(result.value());
        }
    });
}

void ui::connectAndPair(std::string address, ServerCallback<Host> connectResult)
{
    GameStreamClient::client()->connect(address, [address, connectResult](GSResult<SERVER_DATA> result) {
        if (result.isSuccess())
        {
            Host host;
            host.address  = address;
            host.hostname = result.value().hostname ?: "";
            host.mac      = result.value().mac ?: "";

            if (result.value().paired)
            {
                return connectResult(GSResult<Host>::success(host));
            }

            // Pair
            char r_pin[5];
            sprintf(r_pin, "%d%d%d%d",
                (int)rand() % 10, (int)rand() % 10, (int)rand() % 10, (int)rand() % 10);
            auto pin        = std::string(r_pin);
            auto pair_alert = new brls::Dialog("Please enter the following PIN\non the target PC:\n\n" + pin);
            // pair_alert->cancelable = false;
            pair_alert->open();
            GameStreamClient::client()->pair(address, pin, [host, pair_alert, connectResult](auto result) {
                pair_alert->close();

                if (result.isSuccess())
                {
                    brls::Application::notify("Success");

                    return connectResult(GSResult<Host>::success(host));
                }
                else
                {
                    showError(result.error());
                    return connectResult(GSResult<Host>::failure(result.error()));
                }
            });
        }
        else
        {
            showError(result.error());
        }
    });
}

void ui::apps(Host host)
{
    GameStreamClient::client()->applist(host.address, [host](auto result) {
        if (result.isSuccess())
        {
            brls::Application::pushView(new AppListView(host, result.value()));
        }
    });
}