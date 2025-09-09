#pragma once
#include <chrono>
#include <string>
#include <iostream>

class Timer {
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

public:
    Timer();

    void start();
    void stop();
    void print(const std::string& s);
    void stop(const std::string& s);
};