#pragma once

#include <stdexcept>
#include <cstddef>
#include <limits>
#include <vector>

template <typename T>
class SListArray
{
private:
    static constexpr std::size_t npos = std::numeric_limits<std::size_t>::max();

    struct Node
    {
        T value;
        std::size_t next;
    };

    std::size_t freeHead = npos;
    std::size_t head = npos;
    std::size_t m_size = 0;

    std::vector<Node> m_data;

public:
    SListArray() = default;
    ~SListArray();
    SListArray(SListArray &&other);

    T &front();
    const T &front() const;

    void push_front(const T &value);
    void push_front(T &&value);

    void pop_front();
    void clear();

    bool empty() const;
    std::size_t size() const;

    SListArray(const SListArray &other);
    SListArray &operator=(const SListArray &other);
    SListArray &operator=(SListArray &&other);

    class Iterator
    {
    private:
        SListArray *owner;
        std::size_t current;

    public:
        Iterator(SListArray* list, std::size_t index);
        Iterator &operator++();
        T &operator*() const;
        bool operator!=(const Iterator &other) const;
        bool operator==(const Iterator &other) const;
    };
    Iterator begin();
    Iterator end();

    class ConstIterator
    {
    private:
        const SListArray *owner;
        std::size_t current;

    public:
        ConstIterator(const SListArray* list, std::size_t index);
        ConstIterator &operator++();
        const T &operator*() const;
        bool operator!=(const ConstIterator &other) const;
        bool operator==(const ConstIterator &other) const;
    };
    ConstIterator begin() const;
    ConstIterator end() const;
};