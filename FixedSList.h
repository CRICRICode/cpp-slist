#pragma once

#include <stdexcept>
#include <array>
#include <cstddef>
#include <limits>
#include <utility>

template <typename T, std::size_t N>
class FixedSList
{

private:
    static constexpr std::size_t npos = std::numeric_limits<std::size_t>::max();
    static_assert(N > 0, "FixedSList richiede una capacita maggiore di zero");

    struct Node
    {
        T value;
        std::size_t next;
    };

    std::array<Node, N> m_data;
    std::size_t head = npos;
    std::size_t freeHead = 0;
    std::size_t m_size = 0;

    void ResetToEmpty();

public:
    FixedSList();
    ~FixedSList() = default;

    FixedSList(const FixedSList &other) = default;
    FixedSList &operator=(const FixedSList &other) = default;

    FixedSList(FixedSList &&other);
    FixedSList &operator=(FixedSList &&other);

    void push_front(const T &value);
    void push_front(T &&value);
    void pop_front();

    bool empty() const;
    std::size_t size() const;

    void clear();
    const T &front() const;
    T &front();

    bool Validate() const;
    bool full() const;
    bool try_push_front(const T &value);
    bool try_push_front(T &&value);

    class Iterator
    {
    private:
        FixedSList *owner;
        std::size_t current;

    public:
        Iterator(FixedSList *list, std::size_t index);
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
        const FixedSList *owner;
        std::size_t current;

    public:
        ConstIterator(const FixedSList *list, std::size_t index);
        ConstIterator &operator++();
        const T &operator*() const;
        bool operator!=(const ConstIterator &other) const;
        bool operator==(const ConstIterator &other) const;
    };
    ConstIterator begin() const;
    ConstIterator end() const;
};

// private
template <typename T, std::size_t N>
void FixedSList<T, N>::ResetToEmpty()
{
    this->head = npos;
    this->freeHead = 0;
    this->m_size = 0;

    for (std::size_t i = 0; i < N - 1; ++i)
    {
        m_data[i].next = i + 1;
    }

    m_data[N - 1].next = npos;
}

// public
template <typename T, std::size_t N>
FixedSList<T, N>::FixedSList()
{
    ResetToEmpty();
}

template <typename T, std::size_t N>
FixedSList<T, N>::FixedSList(FixedSList &&other) : m_data(std::move(other.m_data)),
                                                   head(other.head),
                                                   freeHead(other.freeHead),
                                                   m_size(other.m_size)
{
    other.ResetToEmpty();
}

template <typename T, std::size_t N>
FixedSList<T, N> &FixedSList<T, N>::operator=(FixedSList &&other)
{
    if (this != &other)
    {
        this->m_data = std::move(other.m_data);
        this->head = other.head;
        this->freeHead = other.freeHead;
        this->m_size = other.m_size;

        other.ResetToEmpty();
    }
    return *this;
}

template <typename T, std::size_t N>
void FixedSList<T, N>::push_front(const T &value)
{
    if (this->freeHead == npos)
    {
        throw std::length_error("FixedSList: list is full");
    }

    std::size_t slot = freeHead;
    this->m_data[slot].value = value;

    this->freeHead = m_data[slot].next;
    this->m_data[slot].next = head;
    this->head = slot;
    this->m_size++;
}

template <typename T, std::size_t N>
void FixedSList<T, N>::push_front(T &&value)
{
    if (this->freeHead == npos)
    {
        throw std::length_error("FixedSList: list is full");
    }

    std::size_t slot = freeHead;
    this->m_data[slot].value = std::move(value);

    this->freeHead = m_data[slot].next;
    this->m_data[slot].next = head;
    this->head = slot;
    this->m_size++;
}

template <typename T, std::size_t N>
void FixedSList<T, N>::pop_front()
{
    if (head == npos)
    {
        throw std::out_of_range("FixedSList: list is empty");
    }
    std::size_t removed = this->head;
    this->head = m_data[removed].next;
    this->m_data[removed].next = freeHead;
    this->freeHead = removed;
    this->m_size--;
}

template <typename T, std::size_t N>
void FixedSList<T, N>::clear()
{
    this->ResetToEmpty();
}

template <typename T, std::size_t N>
bool FixedSList<T, N>::empty() const
{
    return (this->head == npos && this->m_size == 0);
}

template <typename T, std::size_t N>
std::size_t FixedSList<T, N>::size() const
{
    return m_size;
}

template <typename T, std::size_t N>
const T &FixedSList<T, N>::front() const
{
    if (empty())
    {
        throw std::out_of_range("FixedSList is empty");
    }
    return this->m_data.at(this->head).value;
}

template <typename T, std::size_t N>
T &FixedSList<T, N>::front()
{
    if (empty())
    {
        throw std::out_of_range("FixedSList is empty");
    }
    return this->m_data.at(this->head).value;
}

// Iterator

template <typename T, std::size_t N>
FixedSList<T, N>::Iterator::Iterator(FixedSList *list, std::size_t index) : owner(list), current(index) {}

template <typename T, std::size_t N>
T &FixedSList<T, N>::Iterator::operator*() const
{
    if (current == npos)
    {
        throw std::out_of_range("FixedSList : cannot dereference end iterator");
    }
    return this->owner->m_data.at(current).value;
}

template <typename T, std::size_t N>
typename FixedSList<T, N>::Iterator &FixedSList<T, N>::Iterator::operator++()
{
    if (this->current != npos)
    {
        this->current = this->owner->m_data[this->current].next;
    }
    return *this;
}

template <typename T, std::size_t N>
bool FixedSList<T, N>::Iterator::operator==(const Iterator &other) const
{
    return (this->current == other.current && this->owner == other.owner);
}

template <typename T, std::size_t N>
bool FixedSList<T, N>::Iterator::operator!=(const Iterator &other) const
{
    return !(*this == other);
}

template <typename T, std::size_t N>
typename FixedSList<T, N>::Iterator FixedSList<T, N>::begin()
{
    return Iterator{this, this->head};
}

template <typename T, std::size_t N>
typename FixedSList<T, N>::Iterator FixedSList<T, N>::end()
{
    return Iterator{this, npos};
}

// Iterator Const

template <typename T, std::size_t N>
FixedSList<T, N>::ConstIterator::ConstIterator(const FixedSList *list, std::size_t index) : owner(list), current(index) {}

template <typename T, std::size_t N>
const T &FixedSList<T, N>::ConstIterator::operator*() const
{
    if (current == npos)
    {
        throw std::out_of_range("FixedSList : cannot dereference end iterator");
    }
    return this->owner->m_data.at(current).value;
}

template <typename T, std::size_t N>
typename FixedSList<T, N>::ConstIterator &FixedSList<T, N>::ConstIterator::operator++()
{
    if (this->current != npos)
    {
        this->current = this->owner->m_data[this->current].next;
    }
    return *this;
}

template <typename T, std::size_t N>
bool FixedSList<T, N>::ConstIterator::operator==(const ConstIterator &other) const
{
    return (this->current == other.current && this->owner == other.owner);
}

template <typename T, std::size_t N>
bool FixedSList<T, N>::ConstIterator::operator!=(const ConstIterator &other) const
{
    return !(*this == other);
}

template <typename T, std::size_t N>
typename FixedSList<T, N>::ConstIterator FixedSList<T, N>::begin() const
{
    return ConstIterator{this, this->head};
}

template <typename T, std::size_t N>
typename FixedSList<T, N>::ConstIterator FixedSList<T, N>::end() const
{
    return ConstIterator{this, npos};
}

// Extra

template <typename T, std::size_t N>
bool FixedSList<T, N>::Validate() const
{
    if ((head == npos) != (m_size == 0) || m_size > N)
    {
        return false;
    }

    std::array<bool, N> seen{};

    std::size_t activeCount = 0;
    for (std::size_t current = head; current != npos;)
    {
        if (current >= N || seen[current])
        {
            return false;
        }

        seen[current] = true;
        ++activeCount;
        current = m_data[current].next;
    }

    if (activeCount != m_size)
    {
        return false;
    }

    std::size_t freeCount = 0;
    for (std::size_t current = freeHead; current != npos;)
    {
        if (current >= N || seen[current])
        {
            return false;
        }

        seen[current] = true;
        ++freeCount;
        current = m_data[current].next;
    }

    return activeCount + freeCount == N;
}

template <typename T, std::size_t N>
bool FixedSList<T, N>::full() const
{
    return m_size == N;
}

template <typename T, std::size_t N>
bool FixedSList<T, N>::try_push_front(const T &value)
{
    if (full())
    {
        return false;
    }

    push_front(value);
    return true;
}

template <typename T, std::size_t N>
bool FixedSList<T, N>::try_push_front(T &&value)
{
    if (full())
    {
        return false;
    }

    push_front(std::move(value));
    return true;
}