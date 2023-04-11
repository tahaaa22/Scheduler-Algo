#pragma once
#include "StructNode.h"
#include <iostream>

template <typename T>
class SQueue
{
private:
    SNode<T>* front;
    SNode<T>* back;
    int itemCount;

public:
    /*
    * Function: SQueue
    * The SQueue Constructor.
    */
    SQueue() : front(nullptr), back(nullptr), itemCount(0) {}

    /*
    * Function: ~SQueue
    * The SQueue Destructor.
    */
    ~SQueue() 
    {
        clear();
    }

    /*
    * Function: enqueue
    * Inserts a value at the end of the queue.
    *
    * Parameters:
    *  - value: The value to be inserted in the queue.
    */
    void enqueue(T value1, T value2) 
    {
        SNode<T>* temp = new SNode<T>(value1, value2);
        if (!temp) 
        {
            return;
        }
        if (isEmpty()) 
        {
            front = back = temp;
        }
        else 
        {
            back->SetNext(temp);
            back = temp;
        }
        itemCount++;
    }

    /*
    * Function: dequeue
    * Removes the first element in the queue and assigns its values to the given reference variables of type T.
    *
    * Parameters:
    *  - value1: This will hold the value of the first data item in the queue.
    *  - value2: This will hold the value of the second data item in the queue.
    *
    * Returns:
    *  - bool: True if the dequeue operation was successful, False otherwise.
    */
    bool dequeue(T& value1, T& value2)
    {
        if (isEmpty())
        {
            return false;
        }
        value1 = front->getFirstItem();
        value2 = front->getSecondItem();
        SNode<T>* delPtr = front;
        front = front->getNext();
        if (!front) {
            back = nullptr;
        }
        delete delPtr;
        delPtr = nullptr;
        itemCount--;
        return true;
    }

    /*
    * Function: peek
    * Assigns the values of the first element in the queue to the given reference variables of type T.
    *
    * Parameters:
    *  - value1: This will hold the value of the first data item in the queue.
    *  - value2: This will hold the value of the second data item in the queue.
    *
    * Returns:
    *  - bool: True if the peek operation was successful, False otherwise.
    */
    bool peek(T& value1, T& value2) const
    {
        if (isEmpty()) 
        {
            return false;
        }
        value1 = front->getFirstItem();
        value2 = front->getSecondItem();
        return true;
    }

    /*
     Function: size
     Returns the number of elements in the queue.
    
     Returns:
     - int: The number of elements in the queue.
    */
    int size() const
    {
        return itemCount;
    }

    /*
    * Function: isEmpty
    * Determines if the queue is empty or not.
    *
    * Returns:
    *  - bool: True if the queue is empty, False otherwise.
    */
    bool isEmpty() const 
    {
        return itemCount == 0;
    }

    /*
    * Function: clear
    * Removes all the elements from the queue.
    */
    void clear() 
    {
        while (!isEmpty()) {
            T temp1, temp2;
            dequeue(temp1, temp2);
        }
    }
};