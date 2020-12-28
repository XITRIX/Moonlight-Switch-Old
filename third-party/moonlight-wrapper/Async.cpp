#include "Async.hpp"

#include <thread>
#include <mutex>
#include <algorithm>
#include <vector>

static std::mutex m_async_mutex;
static std::vector<std::function<void()>> m_tasks;

static volatile bool task_loop_active = false;

static void task_loop() {
    task_loop_active = true;
    while (task_loop_active) {
        std::vector<std::function<void()>> m_tasks_copy; {
            std::lock_guard<std::mutex> guard(m_async_mutex);
            m_tasks_copy = m_tasks;
            m_tasks.clear();
        }
        
        for (auto task: m_tasks_copy) {
            task();
        }
        
        usleep(500'000);
    }
}

#ifdef __SWITCH__
#include <switch.h>
static Thread task_loop_thread;
static void start_task_loop() {
    threadCreate(
        &task_loop_thread,
        [](void* a) {
            task_loop();
        },
        NULL,
        NULL,
        0x10000,
        0x2C,
        -2
    );
    threadStart(&task_loop_thread);
}
#else
static void start_task_loop() {
    auto thread = std::thread([](){
        task_loop();
    });
    thread.detach();
}
#endif

void perform_async(std::function<void()> task) {
    std::lock_guard<std::mutex> guard(m_async_mutex);
    m_tasks.push_back(task);
}

void Async::run(std::function<void()> task) {
    if (!task_loop_active) {
        start_task_loop();
    }
    perform_async(task);
}

void Async::stop() {
    task_loop_active = false;

#ifdef __SWITCH__
    threadWaitForExit(&task_loop_thread);
    threadClose(&task_loop_thread);
#endif
}