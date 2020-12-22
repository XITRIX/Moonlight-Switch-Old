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

#include <borealis.hpp>
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

int main(int argc, char* argv[])
{
    // Init the app
    brls::Logger::setLogLevel(brls::LogLevel::DEBUG);

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

    brls::List* testList = new brls::List();

    brls::InputListItem* hostIp = new brls::InputListItem("main/add_host/buttons/host_ip"_i18n, "10.0.0.19", "");
    brls::ListItem* connectItem = new brls::ListItem("main/add_host/buttons/connect"_i18n);
    connectItem->getClickEvent()->subscribe([](brls::View* view) {
        
    });

    testList->addView(hostIp);
    testList->addView(new brls::ListItemGroupSpacing());
    testList->addView(connectItem);

    rootFrame->addTab("main/add_host/title"_i18n, testList);

    // Add the root view to the stack
    brls::Application::pushView(rootFrame);

    // Run the app
    while (brls::Application::mainLoop());

    // Exit
    return EXIT_SUCCESS;
}
