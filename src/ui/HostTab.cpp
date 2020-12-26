#include "HostTab.hpp"
#include <streaming/GameStreamClient.hpp>
#include <UIMainScreen.hpp>
#include <ui/UIError.hpp>

namespace i18n = brls::i18n; // for loadTranslations() and getStr()
using namespace i18n::literals; // for _i18n

HostTab::HostTab(Host host, std::function<void(void)> reload) {
    this->host = host;

    auto connect = new brls::ListItem("host_tab/buttons/connect"_i18n);
    auto wakeup = new brls::ListItem("host_tab/buttons/wakeup"_i18n);
    auto unpair = new brls::ListItem("host_tab/buttons/unpair"_i18n);

    addView(unpair);
    addView(new brls::ListItemGroupSpacing());
    addView(wakeup);
    addView(connect);

    connect->getClickEvent()->subscribe([host, reload](auto view) {
        ui::connect(host.address, reload);
    });

    unpair->getClickEvent()->subscribe([host, reload](auto view) {
        GameStreamClient::client()->unpair(host.address, [reload, host](auto result) {
            Settings::settings()->remove_host(host);
            if (result.isSuccess()) {
                showError("Unpaired successfuly");
            } else {
                showError(result.error());
            }
            reload();
        });
    });
}

