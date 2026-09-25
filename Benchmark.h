#pragma once

#include <chrono>
#include <cstdint>
#include <cstddef>
#include <iostream>
#include <string_view>

template <typename List, typename Prepare>
void BenchmarkList(std::string_view name,
                   std::size_t elementCount,
                   std::size_t repetitions,
                   Prepare prepare)
{
    using Clock = std::chrono::steady_clock;

    auto buildTotal = Clock::duration::zero();
    auto traversalTotal = Clock::duration::zero();
    auto clearTotal = Clock::duration::zero();

    std::uint64_t checksum = 0;

    for (std::size_t repetition = 0; repetition < repetitions; ++repetition)
    {
        List list;
        prepare(list);

        auto start = Clock::now();
        for (std::size_t i = 0; i < elementCount; ++i)
        {
            list.push_front(static_cast<int>(i));
        }
        buildTotal += Clock::now() - start;

        const List &view = list;

        start = Clock::now();
        for (auto it = view.begin(); it != view.end(); ++it)
        {
            checksum += static_cast<std::uint64_t>(*it);
        }
        traversalTotal += Clock::now() - start;

        start = Clock::now();
        while (!list.empty())
        {
            list.pop_front();
        }
        clearTotal += Clock::now() - start;
    }

    const auto buildUs =
        std::chrono::duration_cast<std::chrono::microseconds>(buildTotal).count();
    const auto traversalUs =
        std::chrono::duration_cast<std::chrono::microseconds>(traversalTotal).count();
    const auto clearUs =
        std::chrono::duration_cast<std::chrono::microseconds>(clearTotal).count();

    std::cout << name << '\n'
              << "  build:     " << buildUs << " us\n"
              << "  traversal: " << traversalUs << " us\n"
              << "  pop-all:     " << clearUs << " us\n"
              << "  checksum:  " << checksum << "\n\n";
}