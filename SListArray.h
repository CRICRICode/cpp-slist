#pragma once

#include <stdexcept>
#include <cstddef>
#include <limits>
#include <vector>
#include <utility>

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
    ~SListArray() = default;
    SListArray(SListArray &&other);

    T &front();
    const T &front() const;

    void push_front(const T &value);
    void push_front(T &&value);

    void pop_front();
    void clear();

    bool empty() const;
    std::size_t size() const;

    SListArray(const SListArray &other) = default;
    SListArray &operator=(const SListArray &other) = default;
    SListArray &operator=(SListArray &&other);

    bool Validate() const;
    void reserve(std::size_t slotCount);

    class Iterator
    {
    private:
        SListArray *owner;
        std::size_t current;

    public:
        Iterator(SListArray *list, std::size_t index);
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
        ConstIterator(const SListArray *list, std::size_t index);
        ConstIterator &operator++();
        const T &operator*() const;
        bool operator!=(const ConstIterator &other) const;
        bool operator==(const ConstIterator &other) const;
    };
    ConstIterator begin() const;
    ConstIterator end() const;
};

template <typename T>
SListArray<T>::SListArray(SListArray &&other) : freeHead(other.freeHead),
                                                head(other.head),
                                                m_size(other.m_size),
                                                m_data(std::move(other.m_data))

{
    other.freeHead = npos;
    other.head = npos;
    other.m_size = 0;
}

template <typename T>
SListArray<T> &SListArray<T>::operator=(SListArray<T> &&other)
{
    if (this != &other)
    {
        this->m_data = std::move(other.m_data);
        this->freeHead = other.freeHead;
        this->head = other.head;
        this->m_size = other.m_size;
        other.freeHead = npos;
        other.head = npos;
        other.m_size = 0;
    }
    return *this;
}

template <typename T>
bool SListArray<T>::empty() const
{

    return m_size == 0;
}

template <typename T>
std::size_t SListArray<T>::size() const
{
    return m_size;
}

template <typename T>
T &SListArray<T>::front()
{
    if (empty())
    {
        throw std::out_of_range("List is empty");
    }
    return this->m_data.at(this->head).value;
}

template <typename T>
const T &SListArray<T>::front() const
{
    if (empty())
    {
        throw std::out_of_range("List is empty");
    }
    return this->m_data.at(this->head).value;
}

template <typename T>
void SListArray<T>::push_front(const T &value)
{
    std::size_t slot = freeHead;
    if (freeHead != npos)
    {
        this->m_data[slot].value = value;
        freeHead = this->m_data[freeHead].next;
    }
    else
    {
        slot = this->m_data.size();
        this->m_data.emplace_back(Node{value, head});
    }
    this->m_data[slot].next = this->head;
    this->head = slot;
    this->m_size++;
}

template <typename T>
void SListArray<T>::push_front(T &&value)
{
    std::size_t slot = freeHead;
    if (freeHead != npos)
    {
        this->m_data[slot].value = std::move(value);
        freeHead = this->m_data[freeHead].next;
    }
    else
    {
        slot = this->m_data.size();
        this->m_data.emplace_back(Node{std::move(value), head});
    }
    this->m_data[slot].next = this->head;
    this->head = slot;
    this->m_size++;
}

template <typename T>
void SListArray<T>::pop_front()
{
    if (empty())
    {
        throw std::out_of_range("List is empty");
    }
    std::size_t oldHead = head;
    head = this->m_data[head].next;
    this->m_data[oldHead].next = freeHead;
    freeHead = oldHead;
    this->m_size--;
}

template <typename T>
void SListArray<T>::clear()
{
    freeHead = npos;
    head = npos;
    m_size = 0;
    m_data.clear();
}

// Iterator

template <typename T>
SListArray<T>::Iterator::Iterator(SListArray *list, std::size_t index) : owner(list), current(index) {}

template <typename T>
T &SListArray<T>::Iterator::operator*() const
{
    if (current == npos)
    {
        throw std::out_of_range("SListArray: cannot dereference end iterator");
    }
    return this->owner->m_data.at(current).value;
}

template <typename T>
typename SListArray<T>::Iterator &SListArray<T>::Iterator::operator++()
{
    if (this->current != npos)
    {
        this->current = this->owner->m_data[this->current].next;
    }
    return *this;
}

template <typename T>
bool SListArray<T>::Iterator::operator==(const Iterator &other) const
{
    return (this->current == other.current && this->owner == other.owner);
}

template <typename T>
bool SListArray<T>::Iterator::operator!=(const Iterator &other) const
{
    return !(*this == other);
}

template <typename T>
typename SListArray<T>::Iterator SListArray<T>::begin()
{
    return Iterator{this, this->head};
}

template <typename T>
typename SListArray<T>::Iterator SListArray<T>::end()
{
    return Iterator{this, npos};
}

// ConstIterator

template <typename T>
SListArray<T>::ConstIterator::ConstIterator(const SListArray *list, std::size_t index) : owner(list), current(index) {}

template <typename T>
const T &SListArray<T>::ConstIterator::operator*() const
{
    if (current == npos)
    {
        throw std::out_of_range("SListArray: cannot dereference end iterator");
    }
    return this->owner->m_data.at(current).value;
}

template <typename T>
typename SListArray<T>::ConstIterator &SListArray<T>::ConstIterator::operator++()
{
    if (this->current != npos)
    {
        this->current = this->owner->m_data[this->current].next;
    }
    return *this;
}

template <typename T>
bool SListArray<T>::ConstIterator::operator==(const ConstIterator &other) const
{
    return (this->current == other.current && this->owner == other.owner);
}

template <typename T>
bool SListArray<T>::ConstIterator::operator!=(const ConstIterator &other) const
{
    return !(*this == other);
}

template <typename T>
typename SListArray<T>::ConstIterator SListArray<T>::begin() const
{
    return ConstIterator{this, this->head};
}

template <typename T>
typename SListArray<T>::ConstIterator SListArray<T>::end() const
{
    return ConstIterator{this, npos};
}

// Extra

template <typename T>
bool SListArray<T>::Validate() const
{
    if ((head == npos) != (m_size == 0) || m_size > m_data.size())
    {
        return false;
    }

    std::vector<bool> seen(m_data.size(), false);

    std::size_t activeCount = 0;
    for (std::size_t current = head; current != npos;)
    {
        if (current >= m_data.size() || seen[current])
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
        if (current >= m_data.size() || seen[current])
        {
            return false;
        }

        seen[current] = true;
        ++freeCount;
        current = m_data[current].next;
    }

    return activeCount + freeCount == m_data.size();
}

template <typename T>
void SListArray<T>::reserve(std::size_t slotCount)
{
    m_data.reserve(slotCount);
}