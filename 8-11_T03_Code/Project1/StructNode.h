#pragma once
template<typename T>
class SNode
{
private:
	T data1; // first data item (can be any complex sturcture)
	T data2; // second data item (can be any complex sturcture)
	SNode<T>* next; // Pointer to next node
public:

	SNode() //default constructor
	{
		next = nullptr;
	}

	SNode(T Item1, T Item2) //non-default constructor
	{
		data1 = Item1;
		data2 = Item2;
		next = nullptr;

	}

	void setFirstItem(T Item1)
	{
		data1 = Item1;
	} // end set first Item
	void setSecondItem(T Item2)
	{
		data2 = Item2;
	} // end set second Item

	void setNext(SNode <T>* nextNodePtr)
	{
		next = nextNodePtr;
	} // end setNext

	T getFirstItem()
	{
		return data1;
	} // end get first Item

	T getSecondItem()
	{
		return data2;
	} // end getItem

	SNode<T>* getNext()
	{
		return next;
	}
}; // end Node
