#pragma once

#include <stdexcept>
#include <cstddef>
#include <utility>

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

    bool Validate() const;

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

template <typename T>
void SList<T>::clear()
{

    Node *newHead = nullptr;
    while (this->head != nullptr)
    {
        newHead = this->head->next;
        delete (this->head);
        this->head = newHead;
        this->m_size--;
    }
}

template <typename T>
SList<T>::~SList()
{
    clear();
}

template <typename T>
bool SList<T>::empty() const
{
    return (this->head == nullptr && this->m_size == 0);
}

template <typename T>
std::size_t SList<T>::size() const
{
    return m_size;
}

template <typename T>
void SList<T>::push_front(const T &other)
{
    Node *newHead = new Node{other, this->head};
    this->head = newHead;
    this->m_size++;
}

template <typename T>
void SList<T>::push_front(T &&other)
{
    Node *newHead = new Node{std::move(other), this->head};
    this->head = newHead;
    this->m_size++;
}

template <typename T>
T &SList<T>::front()
{
    if (empty())
    {
        throw std::out_of_range("SList is empty");
    }
    T &valueHead = this->head->value;
    return valueHead;
}

template <typename T>
const T &SList<T>::front() const
{
    if (empty())
    {
        throw std::out_of_range("SList is empty");
    }
    const T &valueHead = this->head->value;
    return valueHead;
}

template <typename T>
void SList<T>::pop_front()
{
    if (this->head == nullptr)
    {
        throw std::out_of_range("Slist is empty");
    }

    Node *newHead = this->head->next;
    delete (this->head);
    this->head = newHead;
    m_size--;
}

template <typename T>
void SList<T>::CopyFrom(const SList &other)
{

    const Node *current = other.head;
    Node *tail = nullptr;

    while (current != nullptr)
    {
        Node *newNode = new Node{current->value, nullptr};

        if (tail == nullptr)
        {
            this->head = newNode;
        }
        else
        {
            tail->next = newNode;
        }

        tail = newNode;
        this->m_size++;
        current = current->next;
    }
}

template <typename T>
SList<T>::SList(const SList &other)
{
    CopyFrom(other);
}

template <typename T>
SList<T> &SList<T>::operator=(const SList<T> &other)
{
    if (this != &other)
    {
        this->clear();
        this->CopyFrom(other);
    }
    return *this;
}

template <typename T>
SList<T>::SList(SList &&other)
{
    this->head = other.head;
    this->m_size = other.m_size;

    other.head = nullptr;
    other.m_size = 0;
}

template <typename T>
SList<T> &SList<T>::operator=(SList &&other)
{
    if (this != &other)
    {
        this->clear();
        this->head = other.head;
        this->m_size = other.m_size;

        other.head = nullptr;
        other.m_size = 0;
    }
    return *this;
}


// Iterator

template <typename T>
SList<T>::Iterator::Iterator(typename SList<T>::Node *node) : current(node) {}

template <typename T>
typename SList<T>::Iterator SList<T>::begin()
{
    return Iterator{this->head};
}

template <typename T>
typename SList<T>::Iterator SList<T>::end()
{
    return Iterator{nullptr};
}

template <typename T>
bool SList<T>::Iterator::operator!=(const Iterator &other) const
{
    return this->current != other.current;
}

template <typename T>
bool SList<T>::Iterator::operator==(const Iterator &other) const
{
    return !(this->current != other.current);
}

template <typename T>
typename SList<T>::Iterator &SList<T>::Iterator::operator++()
{
    if (this->current != nullptr)
    {
        this->current = this->current->next;
    }
    return *this;
}

template <typename T>
T &SList<T>::Iterator::operator*() const
{
    if (this->current == nullptr)
    {
        throw std::out_of_range("SList iterator is null");
    }
    return this->current->value;
}

// ConstIterator

template <typename T>
SList<T>::ConstIterator::ConstIterator(const typename SList<T>::Node *node) : current(node) {};

template <typename T>
typename SList<T>::ConstIterator SList<T>::begin() const
{
    return ConstIterator{this->head};
}

template <typename T>
typename SList<T>::ConstIterator SList<T>::end() const
{
    return ConstIterator{nullptr};
}

template <typename T>
typename SList<T>::ConstIterator &SList<T>::ConstIterator::operator++()
{
    if (this->current != nullptr)
    {
        current = this->current->next;
    }
    return *this;
}

template <typename T>
const T &SList<T>::ConstIterator::operator*() const
{
    if (this->current == nullptr)
    {
        throw std::out_of_range("SList iterator is null");
    }

    return this->current->value;
}

template <typename T>
bool SList<T>::ConstIterator::operator!=(const ConstIterator &other) const
{
    return this->current != other.current;
}

template <typename T>
bool SList<T>::ConstIterator::operator==(const ConstIterator &other) const
{
    return !(this->current != other.current);
}

//Extra

template <typename T>
bool SList<T>::Validate() const
{
    if ((head == nullptr) != (m_size == 0))
    {
        return false;
    }

    const Node *current = head;
    std::size_t count = 0;

    while (current != nullptr)
    {
        if (count == m_size)
        {
            return false;
        }

        ++count;
        current = current->next;
    }

    return count == m_size;
}