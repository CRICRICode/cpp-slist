#include "FixedSList.h"
#include "SList.h"
#include "SListArray.h"

#include <iostream>
#include <stdexcept>
#include <string_view>
#include <utility>

namespace
{
    int failures = 0;

    void Check(bool condition, std::string_view label)
    {
        if (condition)
        {
            std::cout << "[PASS] " << label << '\n';
            return;
        }

        std::cout << "[FAIL] " << label << '\n';
        ++failures;
    }

    template <typename Function>
    void CheckOutOfRange(Function operation, std::string_view label)
    {
        try
        {
            operation();
            Check(false, label);
        }
        catch (const std::out_of_range&)
        {
            Check(true, label);
        }
        catch (...)
        {
            Check(false, label);
        }
    }

    template <typename List>
    void SelfAssign(List& value)
    {
        List* alias = &value;
        value = *alias;
    }

    template <typename List>
    void RunCommonListTests(std::string_view name)
    {
        std::cout << '\n' << name << '\n';

        List values;
        Check(values.empty() && values.size() == 0 && values.Validate(),
              "new list is empty and valid");
        CheckOutOfRange([&values] { static_cast<void>(values.front()); },
                        "front on an empty list throws out_of_range");
        CheckOutOfRange([&values] { values.pop_front(); },
                        "pop_front on an empty list throws out_of_range");

        values.push_front(10);
        values.push_front(20);
        values.push_front(30);
        Check(values.front() == 30 && values.size() == 3 && values.Validate(),
              "push_front preserves LIFO order and invariants");

        int sum = 0;
        const List& view = values;
        for (auto it = view.begin(); it != view.end(); ++it)
        {
            sum += *it;
        }
        Check(sum == 60, "const traversal visits every active value");

        values.front() = 99;
        Check(values.front() == 99 && values.Validate(),
              "mutable front returns a writable reference");

        List copy = values;
        copy.front() = 40;
        Check(values.front() == 99 && copy.front() == 40 && copy.Validate(),
              "copy construction creates independent values");

        List assigned;
        assigned.push_front(1);
        assigned = values;
        Check(assigned.front() == 99 && assigned.size() == 3 && assigned.Validate(),
              "copy assignment replaces the destination content");

        SelfAssign(assigned);
        Check(assigned.front() == 99 && assigned.size() == 3 && assigned.Validate(),
              "self copy assignment preserves the list");

        List moved = std::move(copy);
        Check(moved.front() == 40 && moved.size() == 3 && moved.Validate(),
              "move construction transfers the content");
        Check(copy.empty() && copy.size() == 0 && copy.Validate(),
              "moved-from source stays valid and empty");

        List target;
        target.push_front(1);
        target = std::move(moved);
        Check(target.front() == 40 && target.size() == 3 && target.Validate(),
              "move assignment replaces the destination content");
        Check(moved.empty() && moved.size() == 0 && moved.Validate(),
              "move-assignment source stays valid and empty");

        values.pop_front();
        Check(values.front() == 20 && values.size() == 2 && values.Validate(),
              "pop_front advances the head and updates size");

        values.clear();
        Check(values.empty() && values.size() == 0 && values.Validate(),
              "clear restores a valid empty list");
    }

    void RunFixedSListCapacityTests()
    {
        std::cout << "\nFixedSList capacity API\n";

        FixedSList<int, 2> values;
        Check(!values.full() && values.Validate(), "new fixed list is not full");

        const bool firstInserted = values.try_push_front(10);
        const bool secondInserted = values.try_push_front(20);
        Check(firstInserted && secondInserted && values.full() && values.size() == 2 &&
                  values.front() == 20 && values.Validate(),
              "try_push_front fills available slots");

        const bool thirdInserted = values.try_push_front(30);
        Check(!thirdInserted && values.full() && values.size() == 2 && values.front() == 20 &&
                  values.Validate(),
              "try_push_front fails without changing a full list");

        values.pop_front();
        Check(!values.full() && values.size() == 1 && values.front() == 10 && values.Validate(),
              "pop_front releases one fixed slot");

        values.clear();
        Check(values.empty() && !values.full() && values.Validate(),
              "clear restores all fixed slots to the free list");
    }
}

int main()
{
    std::cout << std::boolalpha;

    RunCommonListTests<SList<int>>("SList");
    RunCommonListTests<SListArray<int>>("SListArray");
    RunCommonListTests<FixedSList<int, 8>>("FixedSList");
    RunFixedSListCapacityTests();

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
