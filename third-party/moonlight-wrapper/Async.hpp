#include <vector>
#include <mutex>
#include <algorithm>

class Async {
public:
    void run(std::function<void()> task);
    void stop();
    
    static Async* instance() {
        static Async instant;
        return &instant;
    }

private:
    Async();
};