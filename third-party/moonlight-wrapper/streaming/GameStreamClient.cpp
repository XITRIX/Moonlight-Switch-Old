#include "GameStreamClient.hpp"

#include <unistd.h>

#include <Async.hpp>
#include <algorithm>
#include <borealis.hpp>
#include <fstream>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "Settings.hpp"
#include "WakeOnLanManager.hpp"

GameStreamClient::GameStreamClient()
{
    //start_task_loop();
}

void GameStreamClient::stop()
{
    Async::stop();
}

void GameStreamClient::wake_up_host(const Host& host, ServerCallback<bool> callback)
{
    Async::run([this, host, callback] {
        if (WakeOnLanManager::manager()->wake_up_host(host))
        {
            // usleep(5'000'000);
            // nanogui::async([callback] {  });
            callback(GSResult<bool>::success(true));
        }
        else
        {
            // nanogui::async([callback] {  });
            callback(GSResult<bool>::failure("Wake up failed..."));
        }
    });
}

void GameStreamClient::connect(const std::string& address, ServerCallback<SERVER_DATA> callback)
{
    m_server_data[address] = SERVER_DATA();

    Async::run([this, address, callback] {
        // TODO: mem leak here :(
        int status = gs_init(&m_server_data[address], (char*)(new std::string(address))->c_str(), Settings::settings()->key_dir().c_str(), Settings::settings()->ignore_unsupported_resolutions());

        // nanogui::async([this, address, callback, status] {
        if (status == GS_OK)
        {
            Host host;
            host.address  = address;
            host.hostname = m_server_data[address].hostname ?: "";
            host.mac      = m_server_data[address].mac ?: "";
            Settings::settings()->add_host(host);
            callback(GSResult<SERVER_DATA>::success(m_server_data[address]));
        }
        else
        {
            callback(GSResult<SERVER_DATA>::failure(gs_error()));
        }
        // });
    });
}

void GameStreamClient::pair(const std::string& address, const std::string& pin, ServerCallback<bool> callback)
{
    if (m_server_data.count(address) == 0)
    {
        callback(GSResult<bool>::failure("Firstly call connect()..."));
        return;
    }

    Async::run([this, address, pin, callback] {
        int status = gs_pair(&m_server_data[address], (char*)pin.c_str());
        // nanogui::async([callback, status] {
        if (status == GS_OK)
        {
            callback(GSResult<bool>::success(true));
        }
        else
        {
            callback(GSResult<bool>::failure(gs_error()));
        }
        // });
    });
}

void GameStreamClient::unpair(const std::string& address, ServerCallback<bool> callback)
{
    if (m_server_data.count(address) == 0)
    {
        callback(GSResult<bool>::failure("Firstly call connect()..."));
        return;
    }

    Async::run([this, address, callback] {
        int status = gs_unpair(&m_server_data[address]);
        m_server_data.erase(address);
        if (status == GS_OK)
            callback(GSResult<bool>::success(true));
        else
            callback(GSResult<bool>::failure(gs_error()));
    });
}

void GameStreamClient::applist(const std::string& address, ServerCallback<PAPP_LIST> callback)
{
    if (m_server_data.count(address) == 0)
    {
        callback(GSResult<PAPP_LIST>::failure("Firstly call connect() & pair()..."));
        return;
    }

    m_app_list[address] = PAPP_LIST();

    Async::run([this, address, callback] {
        int status = gs_applist(&m_server_data[address], &m_app_list[address]);

        // nanogui::async([this, address, callback, status] {
        if (status == GS_OK)
        {
            callback(GSResult<PAPP_LIST>::success(m_app_list[address]));
        }
        else
        {
            callback(GSResult<PAPP_LIST>::failure(gs_error()));
        }
        // });
    });
}

void GameStreamClient::app_boxart(const std::string& address, int app_id, ServerCallback<Data> callback)
{
    if (m_server_data.count(address) == 0)
    {
        callback(GSResult<Data>::failure("Firstly call connect() & pair()..."));
        return;
    }

    Async::run([this, address, app_id, callback] {
        Data data;
        int status = gs_app_boxart(&m_server_data[address], app_id, &data);

        // nanogui::async([this, callback, data, status] {
        if (status == GS_OK)
        {
            callback(GSResult<Data>::success(data));
        }
        else
        {
            callback(GSResult<Data>::failure(gs_error()));
        }
        // });
    });
}

void GameStreamClient::start(const std::string& address, STREAM_CONFIGURATION config, int app_id, ServerCallback<STREAM_CONFIGURATION> callback)
{
    if (m_server_data.count(address) == 0)
    {
        callback(GSResult<STREAM_CONFIGURATION>::failure("Firstly call connect() & pair()..."));
        return;
    }

    m_config = config;

    Async::run([this, address, app_id, callback] {
        int status = gs_start_app(&m_server_data[address], &m_config, app_id, Settings::settings()->sops(), Settings::settings()->play_audio(), 0x1);

        // nanogui::async([this, callback, status] {
        if (status == GS_OK)
        {
            callback(GSResult<STREAM_CONFIGURATION>::success(m_config));
        }
        else
        {
            callback(GSResult<STREAM_CONFIGURATION>::failure(gs_error()));
        }
        // });
    });
}

void GameStreamClient::quit(const std::string& address, ServerCallback<bool> callback)
{
    if (m_server_data.count(address) == 0)
    {
        callback(GSResult<bool>::failure("Firstly call connect() & pair()..."));
        return;
    }

    auto server_data = m_server_data[address];

    Async::run([this, server_data, callback] {
        int status = gs_quit_app((PSERVER_DATA)&server_data);

        // nanogui::async([this, callback, status] {
        if (status == GS_OK)
        {
            callback(GSResult<bool>::success(true));
        }
        else
        {
            callback(GSResult<bool>::failure(gs_error()));
        }
        // });
    });
}
