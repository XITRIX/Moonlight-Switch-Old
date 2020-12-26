#pragma once

#include <Settings.hpp>
#include <borealis.hpp>
#include <xml.h>

class AppListView: public brls::AppletFrame 
{
public:
    AppListView(Host host, PAPP_LIST apps);
};