/*
    Borealis, a Nintendo Switch UI Library
    Copyright (C) 2019-2020  natinusala
    Copyright (C) 2019  p-sam

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>

#ifdef __SWITCH__
#include <switch.h>
#endif

#include <EZLogger.hpp>
#include <HostTab.hpp>
#include <Settings.hpp>
#include <UIIngameMenu.hpp>
#include <UIMainScreen.hpp>
#include <borealis.hpp>
#include <chrono>
#include <ctime>
#include <streaming/GameStreamClient.hpp>
#include <streaming/MoonlightSession.hpp>
#include <string>

namespace i18n = brls::i18n; // for loadTranslations() and getStr()
using namespace i18n::literals; // for _i18n

std::vector<std::string> NOTIFICATIONS = {
    "You have cool hair",
    "I like your shoes",
    "borealis is powered by nanovg",
    "The Triforce is an inside job",
    "Pozznx will trigger in one day and twelve hours",
    "Aurora Borealis? At this time of day, at this time of year, in this part of the gaming market, located entirely within your Switch?!",
    "May I see it?",
    "Hmm, Steamed Hams!"
};

std::map<Host, HostTab*> host_menuitems;

bool firstAdd    = true;
bool firstSepAdd = true;
void fillList(brls::TabFrame* frame)
{
    brls::List* addHostTab = new brls::List();

    brls::InputListItem* hostIpItem = new brls::InputListItem("main/add_host/buttons/host_ip"_i18n, "10.0.0.19", "");
    brls::ListItem* connectItem     = new brls::ListItem("main/add_host/buttons/connect"_i18n);
    connectItem->getClickEvent()->subscribe([hostIpItem, frame](brls::View* view) {
        ui::connect(hostIpItem->getValue(), [frame]() { fillList(frame); });
    });

    addHostTab->addView(connectItem);
    addHostTab->addView(new brls::ListItemGroupSpacing());
    addHostTab->addView(hostIpItem);

    auto hosts = Settings::settings()->hosts();
    if (hosts.size() > 0)
    {
        for (auto host = hosts.begin(); host != hosts.end(); host++)
        {
            if (host_menuitems.find(*host.base()) == host_menuitems.end())
            {
                HostTab* new_host            = new HostTab(*host.base(), [frame]() { fillList(frame); });
                host_menuitems[*host.base()] = new_host;
                int index                    = std::distance(hosts.begin(), host);
                frame->addTab(host.base()->hostname, new_host, index);
            }
        }
        if (firstSepAdd)
        {
            frame->addSeparator(frame->tabsCount() - (firstAdd ? 0 : 1));
            firstSepAdd = false;
        }
    }

    if (firstAdd)
        frame->addTab("main/add_host/title"_i18n, addHostTab);

    firstAdd = false;
}

int main(int argc, char* argv[])
{
    srand((unsigned int)std::time(nullptr));

#ifdef __SWITCH__
    appletSetFocusHandlingMode(AppletFocusHandlingMode_NoSuspend);

    bool recording_supported = false;
    appletIsGamePlayRecordingSupported(&recording_supported);

    if (recording_supported)
    {
        appletInitializeGamePlayRecording();
    }
#endif

    // Init the app
    brls::Logger::setLogLevel(brls::LogLevel::DEBUG);

#ifdef __SWITCH__
    Settings::settings()->set_working_dir("sdmc:/switch/moonlight");
#else
    Settings::settings()->set_working_dir("./resources/moonlight");
#endif
    Settings::settings()->set_write_log(true);

    i18n::loadTranslations();
    if (!brls::Application::init("main/name"_i18n))
    {
        brls::Logger::error("Unable to init Borealis application");
        return EXIT_FAILURE;
    }

    // Create a sample view
    brls::TabFrame* rootFrame = new brls::TabFrame();
    rootFrame->setTitle("main/name"_i18n);
    rootFrame->setIcon(BOREALIS_ASSET("icon/borealis.jpg"));

    // rootFrame->registerAction("Test", brls::Key::MINUS, []() {
    //     auto test = new UIIngameMenu([](){});
    //     Application::pushView(test);
    //     return true;
    // });
    fillList(rootFrame);

    // Add the root view to the stack
    brls::Application::pushView(rootFrame);

    // Run the app
    while (brls::Application::mainLoop())
        ;

    GameStreamClient::client()->stop();
    if (MoonlightSession::get_active_session())
    {
        MoonlightSession::get_active_session()->stop(false);
    }

    // Exit
    return EXIT_SUCCESS;
}
