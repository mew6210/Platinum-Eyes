#include "timer.hpp"

Timer::Timer() {
    begin = std::chrono::steady_clock::now();
    end = std::chrono::steady_clock::now();
}

void Timer::start() {
    begin = std::chrono::steady_clock::now();
}

void Timer::stop() {
    end = std::chrono::steady_clock::now();
}

void Timer::print(const std::string& s) {
    std::cout << "["
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
        << "ms] " << s << std::endl;
}

void Timer::stop(const std::string& s) {
    end = std::chrono::steady_clock::now();
    std::cout << "["
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
        << "ms] " << s << std::endl;
}