#pragma once
#include "PQnode.h"
#include <iostream>
using namespace std;


template <typename T>
class PriorityQueue
{
private:

	PQNode<T>* backPtr;
	PQNode<T>* frontPtr;
	int itemCount;

public:
	PriorityQueue();
	bool isEmpty() const;
	int getCount() const;
	bool enqueue(const T& newEntry, const int& prio);
	bool dequeue(T& frntEntry);
	bool peek(T& frntEntry)  const;
	virtual ~PriorityQueue();

	//copy constructor
	PriorityQueue(const PriorityQueue<T>& LQ);

	void Print() const;
	void clear();
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: PriorityQueue()
The constructor of the PriorityQueue class.

*/

template <typename T>
PriorityQueue<T>::PriorityQueue()
{
	backPtr = nullptr;
	frontPtr = nullptr;
	itemCount = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
template <typename T>
bool PriorityQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}

template<typename T>
int PriorityQueue<T>::getCount() const
{
	return itemCount;
}

/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool PriorityQueue<T>::enqueue(const T& newEntry, const int& prio)
{
	PQNode<T>* newNodePtr = new PQNode<T>(newEntry, prio);
	if (!newNodePtr)
		return false;
	// Insert the new node
	if (isEmpty()) //special case if this is the first node to insert
	{
		backPtr = frontPtr = newNodePtr; // The queue is empty
	}

	else if (newNodePtr->getPriority() < frontPtr->getPriority())
	{
		newNodePtr->setNext(frontPtr);
		frontPtr = newNodePtr; // The queue was not empty
	}
	else {
		PQNode<T>* prev = frontPtr;
		PQNode<T>* after = prev->getNext();
		while (after) {
			if (newNodePtr->getPriority() < after->getPriority()) {
				prev->setNext(newNodePtr);
				newNodePtr->setNext(after);
				break;
			}
			prev = prev->getNext();
			after = prev->getNext();
		}
		if (!after) {
			backPtr->setNext(newNodePtr);
			backPtr = newNodePtr;
		}
	}
	itemCount++;
	return true;
} // end enqueue


  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  /*Function: dequeue
  Removes the front of this queue. That is, removes the item that was added
  earliest.

  Input: None.
  Output: True if the operation is successful; otherwise false.
  */

template <typename T>
bool PriorityQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	PQNode<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
		backPtr = nullptr;

	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;

	itemCount--;
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////
/*
Function: peek
copies the front of this queue to the passed param. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
*/
template <typename T>
bool PriorityQueue<T>::peek(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////
/*
Function: destructor
removes all nodes from the queue by dequeuing them
*/
template <typename T>
PriorityQueue<T>::~PriorityQueue()
{
	T temp;

	//Free (Dequeue) all nodes in the queue
	while (dequeue(temp));
}
/////////////////////////////////////////////////////////////////////////////////////////
/*
Function: Copy constructor
To avoid shallow copy,
copy constructor is provided

Input: LinkedQueue<T>: The Queue to be copied
Output: none
*/

template <typename T>
PriorityQueue<T>::PriorityQueue(const PriorityQueue<T>& LQ)
{
	PQNode<T>* NodePtr = LQ.frontPtr;
	if (!NodePtr) //LQ is empty
	{
		frontPtr = backPtr = nullptr;
		return;
	}

	//insert the first node
	PQNode<T>* ptr = new PQNode<T>(NodePtr->getItem());
	frontPtr = backPtr = ptr;
	NodePtr = NodePtr->getNext();

	//insert remaining nodes
	while (NodePtr)
	{
		PQNode<T>* ptr = new PQNode<T>(NodePtr->getItem());
		backPtr->setNext(ptr);
		backPtr = ptr;
		NodePtr = NodePtr->getNext();
	}
	itemCount = LQ.getCount();
}

template <typename T>
void PriorityQueue<T>::Print() const
{
	if (isEmpty())
		return;

	PQNode<T>* curQPtr = frontPtr;
	while (curQPtr)
	{
		cout << curQPtr->getItem();
		if (curQPtr->getNext())
			cout << ", ";
		curQPtr = curQPtr->getNext();
	}
}

template <typename T>
void PriorityQueue<T>::clear()
{
	while (frontPtr)
	{
		T temp;
		dequeue(temp);
	}
	itemCount = 0;
}
