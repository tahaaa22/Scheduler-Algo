#pragma once
#include "Node.h"
#include <iostream>
using namespace std;

template <typename T>
class Queue
{
private:
	Node<T>* front;
	Node<T>* back;
	int itemCount;
public:
	Node<T> * getHead()
	{
		return front;
	}


	/*
	* Function: Queue.
	* The Queue Constructor.
	*/
	Queue()
	{
		front = back = nullptr;
		itemCount = 0;
	}

	/*
	* Function: ~Queue.
	* The Queue Destructor.
	*/
	~Queue()
	{
		/*while (front)
		{
			T temp;
			dequeue(temp);
		}
		itemCount = 0;*/
	}

	/*
	* Function: enqueue.
	* Inserts a value at the beginning of the LinkedList.
	*
	* Parameters:
	*	- value : The value to be inserted in the LinkedList.
	*/
	void enqueue(T value)
	{
		Node<T>* temp = new Node<T>(value);
		if (!temp)
			return;
		if (isEmpty())
		{
			front = back = temp;
			itemCount++;
		}
		else
		{
			back->setNext(temp);
			back = temp;
			itemCount++;
		}
	}

	/*
	* Function: dequeue.
	* Removes the first element in the queue and assigns it to the a given reference variable of type T.
	*
	* Parameters:
	*	- value : This will hold the value of the first element in the queue.
	*/
	void dequeue(T& value)
	{
		if (isEmpty())
			return;
		value = front->getItem();
		Node<T>* delPtr = front;
		front = front->getNext();
		delPtr->setNext(nullptr);
		delete delPtr;
		delPtr = nullptr;
		itemCount--;
	}
	bool dequeueOT(T& value)
	{
		if (isEmpty())
			return false;
		value = front->getItem();
		Node<T>* delPtr = front;
		front = front->getNext();
		delPtr->setNext(nullptr);
		delete delPtr;
		return true;
		delPtr = nullptr;
		itemCount--;
	}
	/*
	* Function: peek.
	* Assigns the value of the first element in the queue to a reference variable of type T.
	*
	* Parameters:
	*	- value : This will hold the value of the first element in the queue.
	*/
	void peek(T& value) const
	{
		if (isEmpty())
			return;
		else
			value = front->getItem();
	}

	/*
	* Function: isEmpty.
	* Checks Whether the queue is empty.
	*
	* Returns: boolean.
	*/
	bool isEmpty() const
	{
		return front == nullptr;
	}

	int getCount() const
	{
		return itemCount;
	}

	void Print() const
	{
		if (isEmpty())
			return;

		Node<T>* curQPtr = front;
		while (curQPtr)
		{
			cout << curQPtr->getItem();
			if (curQPtr->getNext())
				cout << ", ";
			curQPtr = curQPtr->getNext();
		}
	}

	void clear()
	{
		while (front)
		{
			T temp;
			dequeue(temp);
		}
		itemCount = 0;
	}
};