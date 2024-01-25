#ifndef LIST_H
#define LIST_H

template <class T>
struct Node
{
    T info;
    Node<T>* next = nullptr;
    Node<T>* prev = nullptr;
};

template <class T>
class MyIterator
{
    Node<T>* mPointer;
    int index = 0;

public:
    MyIterator (Node<T>* pointer);
    void setPointer(Node<T> *newPointer);
    Node<T>* getPointer();
    T& operator*() const;
    MyIterator& operator++();
    MyIterator& operator++(int);
    MyIterator& operator--();
    T& operator[](int step);
    bool operator==(const MyIterator& secondParam) const;
    bool operator!=(const MyIterator& secondParam) const;
    bool operator<(const MyIterator& secondParam) const;
    bool operator>=(const MyIterator& secondParam) const;
    friend MyIterator<T> operator+(MyIterator<T> iter, int step)
    {
        for (int i = 0; i < step; i++)
            ++iter;
        return iter;
    }
};

template <class T>
class List
{
    Node<T>* head = nullptr;
    Node<T>* tail = nullptr;
    int size=0;
public:
    void DeleteAll();
    void push(T element);
    void deleteElement(int index);
    int getsize();
    //void swap(T& data1, T& data2);
    //void sort(MyIterator<T> first, MyIterator<T> last, bool (*comparator)(T, T));
    MyIterator<T> begin();
    MyIterator<T> end();
    friend class MyIterator<T>;
};

//template<class T>
//void List<T>::sort(MyIterator<T> first, MyIterator<T> last, bool (*comparator)(T, T))
//{
//    int count = 1;
//    for (int i = 0; first[i] != *last; i++)
//        count++;
//    for (int i = 1; i < count; i++)
//        for (int j = i; j > 0 && comparator(first[j - 1], first[j]); j--)
//            algo::swap(first[j - 1], first[j]);
//}

template<class T>
Node<T>* MyIterator<T>::getPointer()
{
    return mPointer;
}

template <class T>
MyIterator<T>::MyIterator(Node<T>* pointer): mPointer(pointer){};

template <class T>
T& MyIterator<T>::operator*() const
{
    return mPointer->info;
}

template <class T>
MyIterator<T>& MyIterator<T>::operator++()
{
    mPointer=mPointer->next;
    return *this;
}

template <class T>
MyIterator<T>& MyIterator<T>::operator++(int)
{
    MyIterator temp = *this;
    mPointer = mPointer->next;
    return temp;
}

template <class T>
MyIterator<T>& MyIterator<T>::operator--()
{
    mPointer = mPointer->prev;
    return *this;
}

template<class T>
T &MyIterator<T>::operator[](int step)
{
    Node<T>* pointer = mPointer;
    for (int i = 0; i < step; i++)
        pointer = pointer->next;
    return pointer->info;
}

template <class T>
bool MyIterator<T>::operator==(const MyIterator& secondParam) const
{
    return mPointer == secondParam.mPointer;
}

template <class T>
bool MyIterator<T>::operator!=(const MyIterator& secondParam) const
{
    return mPointer != secondParam.mPointer;
}

template <class T>
bool MyIterator<T>::operator<(const MyIterator& secondParam) const
{
    return mPointer->info < secondParam.mPointer->info;
}

template <class T>
bool MyIterator<T>::operator>=(const MyIterator& secondParam) const
{
    return index >= secondParam.index;
}


/////////////////////////////////////////////////


template<class T>
void List<T>::DeleteAll()
{
    Node<T>* tmp = tail;
    for (size_t i = size; i > 0; i--)
    {
        if (i == 1)
        {
            head = tail = nullptr;
            size=0;
        }
        else
        {
            Node<T>* tmp = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete tmp;
        }
    }
}

template<class T>
void List<T>::push(T element)
{
    Node<T>* elem = new Node<T>;
    elem->info = element;
    if (!head)
    {
        head = tail = elem;
        head->next = tail->next;
        size++;
    }
    else
    {
        tail->next = elem;
        elem->prev = tail;
        tail = elem;
        size++;
    }
}

template<class T>
int List<T>::getsize()
{
    int s = size;
    return s;
}

template<class T>
void List<T>::deleteElement(int index)
{
    if (!head)
        return;
    Node<T>* temp;
    MyIterator<T> it = head;
    for (int i = 0; i < index - 1; it++, i++);
    temp = it.getPointer();
    if (index > 0)
    {
        Node<T>* temp2 = temp->next;
        if (temp2 == tail)
            tail = 0;
        else
            temp2->next->prev = temp;
        temp->next = temp->next->next;
        delete temp2;
    }
    else
    {
        if (head == tail)
            tail = head = nullptr;
        else
        {
            head = head->next;
            head->prev = nullptr;
        }
        delete temp;
    }
}

template<class T>
MyIterator<T> List<T>::begin()
{
    return MyIterator<T>(head);
}

template<class T>
MyIterator<T> List<T>::end()
{
    return MyIterator<T>(tail);
}

//////////////////////////////

namespace algo
{
    template<class T>
    void swap(T& data1, T& data2)
    {
        T data3 = data1;
        data1 = data2;
        data2 = data3;
    }

    template<class T>
    void sort(MyIterator<T> first, MyIterator<T> last, bool (*comparator)(T, T))
    {
        int count = 1;
        for (int i = 0; first[i] != *last; i++)
            count++;
        for (int i = 1; i < count; i++)
            for (int j = i; j > 0 && comparator(first[j - 1], first[j]); j--)
                swap(first[j - 1], first[j]);
    }

    template<class T>
    void editElement(MyIterator<T>& iter, T substitute)
    {
        iter.getPointer()->info = substitute;
    }
}

#endif // LIST_H
