#pragma once
#include "Node.h"
#include <iostream> 
using namespace std;

template<class T>
class LinkedList
{
public:
	Node<T>* head;
	LinkedList() : head(nullptr) {}
	bool isEmpty() const { return head == nullptr; }
	/*Node* GetHead() { return head; }*/
	int getCount() const
	{
		int count = 0;
		Node<T>* temp = head;
		while (temp != nullptr)
		{
			count++;
			temp = temp->getNext();
		}
		return count;
	}

	void insertNode(const T& newval)
	{
		Node<T>* newnode = new Node<T>();
		newnode->getItem() = newval;
		if (isEmpty())
		{
			newnode->getNext() = nullptr;
			head = newnode;
		}
		else
		{
			newnode->getNext() = head;
			head = newnode;
		}
	}
	bool isFound(const T& key) const
	{
		Node<T>* temp = head;
		bool found = false;
		while (temp != nullptr)
		{
			if (temp->getItem() == key)
			{
				found = true;
				break;
			}
			temp = temp->getNext();
		}
		return found;
	}

	void insertBefore(const T& item, const T& newitem)
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
	}

	void insertEnd(const T& item)
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
	}

	bool deleteEnd()
	{
		if (isEmpty())
		{return false;}
		else
		{
			Node<T>* temp = head;
			while (temp->getNext()->getNext())  // by end of loop temp will point at node before last node
			{
				temp = temp->getNext();
			}
			delete temp->getNext();
			temp->setNext(nullptr);
			return true;
		}

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

	Node* GetLastNode()
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
	}
	void display(node<T>* head) 
	{
    node<T>* temp = head;
    while (temp != nullptr) {
        cout << temp->getItem() << " ";
        temp = temp->getNext();
    }
}

	 ~LinkedList() 
	 { 
		 clear(); 
	 }

};

