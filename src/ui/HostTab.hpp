#pragma once

#include <Settings.hpp>
#include <borealis.hpp>
#include <functional>

class HostTab: public brls::List {
public:
    Host host;
    HostTab(Host host, std::function<void(void)> reload);
};
