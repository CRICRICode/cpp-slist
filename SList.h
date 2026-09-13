#pragma once

#include <stdexcept>
#include <cstddef>

template <typename T>
class SList
{
private:
    struct Node
    {
        T value;
        Node *next;
    };

    Node *head = nullptr;
    std::size_t m_size = 0;

    void CopyFrom(const SList &other);

public:
    SList() = default;
    ~SList();
    SList(SList &&other);

    T &front();
    const T &front() const;

    void push_front(const T &value);
    void push_front(T &&value);

    void pop_front();
    void clear();

    bool empty() const;
    std::size_t size() const;

    SList(const SList &other);
    SList &operator=(const SList &other);
    SList &operator=(SList &&other);

    class Iterator
    {
    private:
        Node *current;

    public:
        Iterator(Node *node);
        Iterator &operator++(); // pre-incremento
        T &operator*() const;
        bool operator!=(const Iterator &other) const;
        bool operator==(const Iterator &other) const;
    };
    Iterator begin();
    Iterator end();

    class ConstIterator
    {
    private:
        const Node *current;

    public:
        ConstIterator(const Node *node);
        ConstIterator &operator++();
        const T &operator*() const;
        bool operator!=(const ConstIterator &other) const;
        bool operator==(const ConstIterator &other) const;
    };
    ConstIterator begin() const;
    ConstIterator end() const;
};