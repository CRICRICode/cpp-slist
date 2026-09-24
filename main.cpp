#include "SList.h"
#include "SListArray.h"
#include <iostream>
#include <stdexcept>

int main()
{
    // SList<int> first;
    // first.push_front(10);
    // first.push_front(20);
    // first.push_front(30);

    // SList<int> copy = first;

    // first.pop_front();

    // std::cout << first.front() << ' ' << first.size() << '\n';
    // std::cout << copy.front() << ' ' << copy.size() << '\n';

    // SList<int> second;
    // second.push_front(999);

    // second = first;
    // std::cout << second.front() << ' ' << second.size() << '\n';

    // // second = second;
    // // std::cout << second.front() << ' ' << second.size() << '\n';

    // SList<int> moved = std::move(first);

    // // std::cout << moved.front() << ' ' << moved.size() << '\n';
    // // std::cout << std::boolalpha << first.empty() << ' ' << first.size() << '\n';

    // SList<int> target;
    // target.push_front(999);

    // target = std::move(moved);
    // std::cout << target.front() << ' ' << target.size() << '\n';
    // std::cout << std::boolalpha << moved.empty() << ' ' << moved.size() << '\n';

    // target = std::move(target);
    // std::cout << target.front() << ' ' << target.size() << '\n';

    // SList<int> values;
    // values.push_front(10);

    // auto it = values.begin();
    // values.pop_front();

    // std::cout << *it << '\n';

    // SListArray<int> values;

    // values.push_front(10);
    // values.push_front(20);

    // std::cout << values.front() << ' ' << values.size() << '\n';

    // SListArray<int> values;

    // values.push_front(10);
    // values.push_front(20);

    // values.pop_front();
    // std::cout << values.front() << ' ' << values.size() << '\n';

    // values.push_front(30);
    // std::cout << values.front() << ' ' << values.size() << '\n';

    // values.clear();
    // std::cout << std::boolalpha
    //           << values.empty() << ' ' << values.size() << '\n';

    // try
    // {
    //     values.pop_front();
    // }
    // catch (const std::out_of_range &error)
    // {
    //     std::cout << error.what() << '\n';
    // }

    SListArray<int> values;

    values.push_front(10);
    values.push_front(20);
    values.push_front(30);

    for (auto it = values.begin(); it != values.end(); ++it)
    {
        std::cout << *it << ' ';
    }
    std::cout << '\n';

    auto saved = values.begin();
    values.push_front(40);

    std::cout << *saved << '\n';

    const SListArray<int> &view = values;

    for (auto it = view.begin(); it != view.end(); ++it)
    {
        std::cout << *it << ' ';
    }
    std::cout << '\n';
}