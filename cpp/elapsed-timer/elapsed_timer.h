/// SPDX-License-Identifier: MIT

#pragma once

#include <chrono>
#include <iostream>

struct ElapsedTimer
{
    ElapsedTimer(const std::string &name = {})
        : m_name(name)
        , m_start(std::chrono::high_resolution_clock::now())
    {
    }

    ~ElapsedTimer()
    {
        std::cout << "ElapsedTimer(" << m_name << ") took " << elapsed() << "ms\n";
    }

    long long elapsed()
    {
        const auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count();
    }

    void print_delta(const std::string &subName)
    {
        auto e = elapsed();
        std::cout << "ElapsedTimer(" << m_name << "," << subName
                  << ") took " << e - m_lastElapsed << "ms\n";

        m_lastElapsed = e;
    }

    std::string m_name;
    std::chrono::system_clock::time_point m_start;
    long long m_lastElapsed = 0;
};
