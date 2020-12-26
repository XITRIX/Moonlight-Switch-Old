#include "AppListView.hpp"
#include <streaming/GameStreamClient.hpp>
#include <ui/UIStreamView.hpp>

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
            auto stream = new UIStreamView(host.address, app->id);
            brls::Application::pushView(stream);
        });
        listView->addView(appItem);
        app = app->next;
    }

    setContentView(listView);
}