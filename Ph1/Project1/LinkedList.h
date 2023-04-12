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
	LinkedList() : head(nullptr), tail(nullptr) { count = 0; }

	bool isEmpty() const { return (tail == nullptr); }

	Node* GetHead() { return head; }
	Node* GetTail() { return tail; }

	int getCount() const   //number of processes in the list
	{
		return count;
	}

	bool deleteNode(Node<T>* to_delete = head) //by default, delete from head equivalent to dequeue, can delete certain node
	{
		if (isEmpty() || !isFound(to_delete)) //if the list is empty or the process is not in the list
		{
			return false;
		}
		else if (to_delete == tail) //if it acts as a normal dequeue
		{
			Node<T>* temp = to_delete;
			to_delete = to_delete->getNext();
			delete temp;
			count--;
			return true;
		}
		else //if a certain process is specified to be deleted 
		{
			Node<T>* before_todelete;
			bool x = isFound(to_delete, &before_todelete);
			Node<T>* temp = to_delete;
			before_todelete->setNext(to_delete->getNext());
			delete temp;
			count--;
			return true;
		}

	}
	void insertNode(Node<T>* p)  //insert at tail and equivalent to enqueue
	{
		if (isEmpty())
		{
			p->getNext() = nullptr;
			tail = p;
			head = p;
			count++;
			return;
		}
		else
		{
			tail->setNext(p);
			p->setNext(nullptr);
			tail = p;
			count++;
			return;
		}
	}

	bool isFound(Node<T>* p, Node<T>& before_p = nullptr) const
	{
		Node<T>* temp = head;
		bool found = false;
		while (temp->getNext() != nullptr)
		{
			before_p = temp;
			if (temp->getNext() == p)
			{
				found = true;
				break;
			}
			temp = temp->getNext();
		}
		return found;
	}

	void clear()
	{
		while (head != nullptr)
		{
			Node<T>* temp = head;
			head = head->getNext();
			delete temp;
		}
	}

	void Print()
	{
		Node<T>* temp = head;
		while (temp)
		{
			cout << temp->getItem() << endl;
			temp = temp->getNext();
		}
		return;
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
	}

	int getOccurrence(const T& item) const
	{
		int count = 0;
		Node<T>* temp = head;
		while (temp != nullptr)
		{
			if (temp->setItem() == item)
				count++;
			temp = temp->getNext();
		}
		return count;
	}

	bool deleteNodes(const T& item)
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
				temp = temp->getNext()
			}

			temp1 = temp->getNext();
			if (temp1 == nullptr)
				temp->getNext() = nullptr;
			else
				temp->getNext() = temp1->getNext();
			if (item == temp1->getItem())
				x--;
			delete temp1;
		}
		return 1;
	}*/


	/*Node* GetLastNode()
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


};

