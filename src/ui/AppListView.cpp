#include "AppListView.hpp"
#include <streaming/GameStreamClient.hpp>
#include <ui/UIStreamView.hpp>
#include <EZLogger.hpp>

namespace i18n = brls::i18n; // for loadTranslations() and getStr()
using namespace i18n::literals; // for _i18n

AppListView::AppListView(Host host, PAPP_LIST apps)
    : AppletFrame(false, true)
{
    setTitle(host.hostname);
    auto listView = new brls::List();

    PAPP_LIST app = apps;
    while (app != NULL)
    {
        auto appItem = new brls::ListItem(app->name);
        appItem->getClickEvent()->subscribe([host, app](auto view) {
            auto stream = new UIStreamView(&host, (PAPP_LIST)app);
            brls::Application::pushView(stream);
        });
        listView->addView(appItem);
        app = app->next;
    }

    setContentView(listView);
}