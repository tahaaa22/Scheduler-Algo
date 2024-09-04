#pragma once
template<typename T>
class Node
{
private:
	T data;	// A data item (can be any complex sturcture)
	Node<T>* next;	// Pointer to next node
public:

	Node() //default constructor
	{
		next = nullptr;
	}

	Node(T newItem) //non-default constructor
	{
		data = newItem;
		next = nullptr;

	}

	void setItem(T newItem)
	{
		data = newItem;
	} // end setItem

	void setNext(Node<T>* nextNodePtr)
	{
		next = nextNodePtr;
	} // end setNext

	T getItem() const
	{
		return data;
	} // end getItem

	Node<T>* getNext() const
	{
		return next;
	}
}; // end Node