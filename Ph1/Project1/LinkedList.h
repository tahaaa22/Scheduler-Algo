#pragma once
#include "Node.h"
#include <iostream> 
using namespace std;

template<class T>
class LinkedList
{
public:
	Node<T>* head;
	Node<T>* tail;
	int count;
	void Test() //delete later
	{
		if (tail)
		{
			cout << "printing tail " << tail->getItem() << endl;
		}
		else
		{
			cout << "no tail" << endl;
		}
		if (head)
		{
			cout << "printing Head " << head->getItem() << endl;
		}
		else
		{
			cout << "no head" << endl;
		}
	}

	LinkedList() : head(nullptr), tail(nullptr), count(0) {}

	bool isEmpty() const { return (head == nullptr); }

	Node <T> *getHead() { return head; }
	T * getHead1() { return head; }
	Node <T> * getTail() { return tail; }

	int getCount() const { return count; }

	bool deleteNode(T& p)
	{
		if (isEmpty()) // if the list is empty
		{
			return false;
		}

		if (head->getItem() == p) // if the node to be deleted is the head node
		{
			Node<T>* temp = head;
			head = head->getNext();
			delete temp;
			count--;
			if (!head) // if the list becomes empty
			{
				tail = nullptr;
			}
			return true;
		}

		Node<T>* temp = head;
		while (temp->getNext() && temp->getNext()->getItem() != p) // traverse the list
		{
			temp = temp->getNext();
		}

		if (!temp->getNext()) // if the node to be deleted is not found in the list
		{
			return false;
		}

		Node<T>* nodeToDelete = temp->getNext();
		temp->setNext(nodeToDelete->getNext());
		if (!nodeToDelete->getNext()) // if the node to be deleted is the tail node
		{
			tail = temp;
		}
		delete nodeToDelete;
		count--;
		return true;
	}

	bool deleteNode() // similar to dequeue
	{
		if (isEmpty()) // if the list is empty 
		{
			return false;
		}
		else // it acts as a normal dequeue
		{
			Node<T>* temp = head;
			if (head == tail) // if list has only 1 element
			{
				tail = nullptr;
			}
			head = head->getNext();
			delete temp;
			count--;
			return true;
		}
	}
	T* getnext(Node<T>* p)
	{
		return p->getNext();
	}
	
	void insertNode( T  p)  // insert at tail and equivalent to enqueue
	{
		Node<T>* Process = new Node<T>;
		Process->setItem(p);
		if (isEmpty())
		{
			Process->setNext(nullptr);
			tail = Process;
			head = Process;
		}
		else
		{
			tail->setNext(Process);
			Process->setNext(nullptr);
			tail = Process;
		}
		count++;
	}

	bool isFound(const T& p) // test
	{
		if (isEmpty()) // if the list is empty
		{
			return false;
		}

		Node<T>* temp = head;
		while (temp != nullptr)
		{
			if (temp->getItem() == p)
			{
				return true;
			}
			temp = temp->getNext();
		}
		return false;
	}

	void clear()
	{
		while (head != nullptr)
		{
			Node<T>* temp = head;
			head = head->getNext();
			delete temp;
		}
		tail = nullptr;
		count = 0;
	}

	void Print()
	{
		Node<T>* temp = getHead();
		while (temp)
		{
			cout << temp->getItem() << endl;
			temp = temp->getNext();
		}
	}




	~LinkedList()
	{
		clear();
	}

	/*void insertBefore(const T& item, const T& newitem)
	{
		if (isFound(item))
		{
			Node<T>* node1 = new Node<T>();
			node1->getItem() = newitem;
			Node<T>* temp = head;
			while (temp != nullptr && temp->getNext()->getItem() != item)
			{
				temp = temp->getNext();
			}
			node1->getNext() = temp->getNext();
			temp->getNext() = node1;
		}
	}*/

	/*void insertEnd(const T& item)
	{
		Node<T>* temp = head;
		while (temp->getNext() != nullptr)
		{
			temp = temp->getNext();
		}
		Node<T>* node1 = new Node<T>();
		node1->getItem() = item;
		temp->getNext() = node1;
		node1->getNext() = nullptr;
	}*/

	/*Node* GetNode(T& p)
	{
		Node<T>* temp = head; //ptr used for traversing the linkedList
		if (isEmpty()) //empty linked list
		{
			return nullptr;
		}
		while (temp->getNext())  //by th end of the loop temp will point to the last node
		{
			temp = temp->getNext();
		}
		return temp;
	}*/

	/*int getOccurrence(const T& item) const
	{
		int count = 0;
		Node<T>* temp = head;
		while (temp != nullptr)
		{
			if (temp->getItem() == item)
				count++;
			temp = temp->getNext();
		}
		return count;
	}
	bool deleteNode(const T& item)
	{
		if (isEmpty())
		{
			return 0;
		}
		Node<T>* temp, * temp1;
		temp = head;
		temp1 = nullptr;
		int x = getOccurrence(item);
		while (x)
		{
			while (temp->getNext() != nullptr && temp->getNext()->getItem() != item)
			{
				temp = temp->getNext();
			}
			temp1 = temp->getNext();
			if (temp1 == nullptr)
				temp->setNext(nullptr);
			else
				temp->setNext(temp1->getNext());
			if (item == temp1->getItem())
				x--;
			delete temp1;
		}
		return 1;
	}*/


};


