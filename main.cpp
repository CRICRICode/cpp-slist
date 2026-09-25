#include "Benchmark.h"
#include "SList.h"
#include "SListArray.h"
#include "FixedSList.h"
#include <iostream>

#include "FixedSList.h"

void PrintState(const FixedSList<int, 2> &list, const char *label)
{
    std::cout << label
              << " | valid: " << list.Validate()
              << " | full: " << list.full()
              << " | size: " << list.size();

    if (!list.empty())
    {
        std::cout << " | front: " << list.front();
    }

    std::cout << '\n';
}

int main()
{
    constexpr std::size_t elementCount = 10'000;
    constexpr std::size_t repetitions = 100;

    BenchmarkList<SList<int>>(
        "SList raw",
        elementCount,
        repetitions,
        [](auto &) {});

    BenchmarkList<SListArray<int>>(
        "SListArray",
        elementCount,
        repetitions,
        [](auto &) {});

    BenchmarkList<SListArray<int>>(
        "SListArray reserved",
        elementCount,
        repetitions,
        [](auto &list)
        {
            list.reserve(elementCount);
        });

    BenchmarkList<FixedSList<int, elementCount>>(
        "FixedSList",
        elementCount,
        repetitions,
        [](auto &) {});

    std::cout << std::boolalpha;

    FixedSList<int, 2> values;

    PrintState(values, "Constructed");

    const bool firstInserted = values.try_push_front(10);
    const bool secondInserted = values.try_push_front(20);
    std::cout << "First two inserts: "
              << firstInserted << ' ' << secondInserted << '\n';
    PrintState(values, "After two pushes");

    const bool thirdInserted = values.try_push_front(30);
    std::cout << "Third insert: " << thirdInserted << '\n';
    PrintState(values, "After failed third push");

    values.pop_front();
    PrintState(values, "After pop");

    values.clear();
    PrintState(values, "After clear");

    FixedSList<int, 2> source;
    source.push_front(10);
    source.push_front(20);

    FixedSList<int, 2> target = std::move(source);
    std::cout << target.front() << std::endl;
}