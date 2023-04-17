#pragma once
#ifndef _PQNODE
#define _PQNODE
template < typename T>
class PQNode
{
private:
	T item; // A data item
	PQNode<T>* next; // Pointer to next node
	int priority;
public:
	PQNode();
	PQNode(const T& r_Item, const int& prio);
	PQNode(const T& r_Item, PQNode<T>* nextNodePtr, const int& prio);
	void setItem(const T& r_Item);
	void setNext(PQNode<T>* nextNodePtr);
	void setPriority(const int& prio);
	T getItem() const;
	PQNode<T>* getNext() const;
	int getPriority() const;
}; // end Node
#endif

template < typename T>
PQNode<T>::PQNode() : priority(0)
{
	next = nullptr;
}

template < typename T>
PQNode<T>::PQNode(const T& r_Item, const int& prio)
{
	item = r_Item;
	next = nullptr;
	priority = prio;
}

template < typename T>
PQNode<T>::PQNode(const T& r_Item, PQNode<T>* nextNodePtr, const int& prio)
{
	item = r_Item;
	next = nextNodePtr;
	priority = prio;
}
template < typename T>
void PQNode<T>::setItem(const T& r_Item)
{
	item = r_Item;
}

template < typename T>
void PQNode<T>::setNext(PQNode<T>* nextNodePtr)
{
	next = nextNodePtr;
}

template < typename T>
void PQNode<T>::setPriority(const int& prio)
{
	priority = prio;
}

template < typename T>
T PQNode<T>::getItem() const
{
	return item;
}

template < typename T>
PQNode<T>* PQNode<T>::getNext() const
{
	return next;
}

template < typename T>
int PQNode<T>::getPriority() const
{
	return priority;
}