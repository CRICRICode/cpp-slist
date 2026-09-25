#include "SList.h"
#include "SListArray.h"
#include "FixedSList.h"
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

    // SListArray<int> values;

    // values.push_front(10);
    // values.push_front(20);
    // values.push_front(30);

    // for (auto it = values.begin(); it != values.end(); ++it)
    // {
    //     std::cout << *it << ' ';
    // }
    // std::cout << '\n';

    // auto saved = values.begin();
    // values.push_front(40);

    // std::cout << *saved << '\n';

    // const SListArray<int> &view = values;

    // for (auto it = view.begin(); it != view.end(); ++it)
    // {
    //     std::cout << *it << ' ';
    // }
    // std::cout << '\n';

    // SListArray<int> first;
    // first.push_front(10);
    // first.push_front(20);
    // first.push_front(30);

    // SListArray<int> copy = first;
    // first.pop_front();

    // std::cout << first.front() << ' ' << first.size() << '\n';
    // std::cout << copy.front() << ' ' << copy.size() << '\n';

    // SListArray<int> moved = std::move(copy);

    // std::cout << moved.front() << ' ' << moved.size() << '\n';
    // std::cout << std::boolalpha << copy.empty() << ' ' << copy.size() << '\n';

    // copy.push_front(99);
    // std::cout << copy.front() << ' ' << copy.size() << '\n';

    // SListArray<int> source;
    // source.push_front(1);
    // source.push_front(2);

    // SListArray<int> destination;
    // destination.push_front(999);

    // destination = source;
    // source.pop_front();

    // std::cout << source.front() << ' ' << source.size() << '\n';
    // std::cout << destination.front() << ' ' << destination.size() << '\n';

    // SListArray<int> moveTarget;
    // moveTarget.push_front(777);

    // moveTarget = std::move(destination);

    // std::cout << moveTarget.front() << ' ' << moveTarget.size() << '\n';
    // std::cout << std::boolalpha
    //           << destination.empty() << ' ' << destination.size() << '\n';

    // SListArray<int> values;
    // values.push_front(10);
    // values.push_front(20);

    // auto saved = values.begin();
    // values.pop_front();

    // std::cout << *saved << '\n';

    // values.push_front(99);
    // std::cout << *saved << '\n';

    // const SListArray<int>& view = values;
    // *view.begin() = 99;

    // FixedSList<int, 4> values;

    // values.push_front(10);
    // values.push_front(20);
    // values.push_front(30);
    // values.push_front(40);

    // std::cout << values.front() << ' ' << values.size() << '\n';

    // try
    // {
    //     values.push_front(50);
    // }
    // catch (const std::length_error &error)
    // {
    //     std::cout << error.what() << '\n';
    // }

    // values.pop_front();
    // std::cout << values.front() << ' ' << values.size() << '\n';

    // values.clear();
    // std::cout << std::boolalpha
    //           << values.empty() << ' ' << values.size() << '\n';

    // try
    // {
    //     std::cout << values.front() << '\n';
    // }
    // catch (const std::out_of_range &error)
    // {
    //     std::cout << error.what() << '\n';
    // }

    // FixedSList<int, 4> source;
    // source.push_front(10);
    // source.push_front(20);
    // source.push_front(30);

    // FixedSList<int, 4> copy = source;
    // source.pop_front();

    //     // std::cout << source.front() << ' ' << source.size() << '\n';
    //     // std::cout << copy.front() << ' ' << copy.size() << '\n';

    //     // FixedSList<int, 4> moved = std::move(copy);

    //     // std::cout << moved.front() << ' ' << moved.size() << '\n';
    //     // std::cout << std::boolalpha << copy.empty() << ' ' << copy.size() << '\n';

    //     // copy.push_front(99);
    //     // std::cout << copy.front() << ' ' << copy.size() << '\n';

    //     // FixedSList<int, 4> values;
    //     // int ten = 10;

    //     // values.push_front(ten); // const T&
    //     // values.push_front(20);  // T&&

    //     // std::cout << values.front() << ' ' << values.size() << '\n';

    //     FixedSList<int, 4> values;

    //     values.push_front(10);
    //     values.push_front(20);
    //     values.push_front(30);

    //     for (auto it = values.begin(); it != values.end(); ++it)
    //     {
    //         std::cout << *it << ' ';
    //     }
    //     std::cout << '\n';
    // //

    // FixedSList<int, 4> values;
    // values.push_front(10);
    // values.push_front(20);
    // values.push_front(30);

    // for (int &value : values)
    // {
    //     value += 1;
    // }

    // const FixedSList<int, 4> &view = values;

    // for (const int &value : view)
    // {
    //     std::cout << value << ' ';
    // }
    // std::cout << '\n';

    // FixedSList<int, 4> first;
    // first.push_front(10);
    // first.push_front(20);
    // first.push_front(30);

    // FixedSList<int, 4> copy = first;
    // first.pop_front();

    // std::cout << first.front() << ' ' << first.size() << '\n';
    // std::cout << copy.front() << ' ' << copy.size() << '\n';

    // FixedSList<int, 4> moved = std::move(copy);

    // std::cout << moved.front() << ' ' << moved.size() << '\n';
    // std::cout << std::boolalpha << copy.empty() << ' ' << copy.size() << '\n';

    // copy.push_front(99);
    // std::cout << copy.front() << ' ' << copy.size() << '\n';

    // FixedSList<int, 4> target;
    // target.push_front(777);
    // target = std::move(moved);

    // std::cout << target.front() << ' ' << target.size() << '\n';
    // std::cout << std::boolalpha << moved.empty() << ' ' << moved.size() << '\n';
}