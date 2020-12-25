#include "HostTab.hpp"
#include <streaming/GameStreamClient.hpp>
#include <ui/UIError.hpp>

namespace i18n = brls::i18n; // for loadTranslations() and getStr()
using namespace i18n::literals; // for _i18n

HostTab::HostTab(Host host, std::function<void(void)> reload) {
    this->host = host;

    auto connect = new brls::ListItem("host_tab/buttons/connect"_i18n);
    auto unpair = new brls::ListItem("Unpair"_i18n);

    addView(connect);
    addView(unpair);

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

