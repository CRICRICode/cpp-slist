#include "Benchmark.h"
#include "FixedSList.h"
#include "SList.h"
#include "SListArray.h"

int main()
{
    constexpr std::size_t elementCount = 10'000;
    constexpr std::size_t repetitions = 100;

    BenchmarkList<SList<int>>(
        "SList raw",
        elementCount,
        repetitions,
        [](auto&) {});

    BenchmarkList<SListArray<int>>(
        "SListArray",
        elementCount,
        repetitions,
        [](auto&) {});

    BenchmarkList<SListArray<int>>(
        "SListArray reserved",
        elementCount,
        repetitions,
        [](auto& list)
        {
            list.reserve(elementCount);
        });

    BenchmarkList<FixedSList<int, elementCount>>(
        "FixedSList",
        elementCount,
        repetitions,
        [](auto&) {});
}
