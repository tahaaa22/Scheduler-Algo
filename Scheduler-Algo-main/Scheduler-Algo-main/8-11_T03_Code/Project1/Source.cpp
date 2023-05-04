#include <iostream>
using namespace std;
#include "LinkedList.h"

int main()
{
	//Test Cases
	//Insertion is working fine
	LinkedList<int> L1;
	int x = 1;
	L1.insertNode(x);
	cout << "Printing after insertion " << x << endl;
	L1.Print();
	x++;
	L1.insertNode(x);
	cout << "Printing after insertion " << x << endl;
	L1.Print();
	x++;
	L1.insertNode(x);
	cout << "Printing after insertion " << x << endl;
	L1.Print();
	x++;
	L1.insertNode(x);
	cout << "Printing after insertion " << x << endl;
	L1.Print();
	x++;
	L1.insertNode(x);
	cout << "Printing after insertion " << x << endl;
	L1.Print();

	//TESTING DELETE
	//deleting at the head does not work
	cout << "x= " << x << endl;
	L1.deleteNode(x);
	cout << "Printing after deletion 1, "<<x<<" should be deleted , this is deleting tail" << endl;
	L1.Print();
	L1.Test();
	x = 1;
	L1.deleteNode(x); 
	cout << "Printing after deletion 2, "<<x<< " should be deleted , this is deleting head value" << endl;
	L1.Print();
	L1.Test();
	x = 3;
	L1.deleteNode(x);
	cout << "Printing after deletion 3, "<<x<<" should be deleted , deleting from the middle" << endl;
	L1.Print();
	L1.Test();
	L1.deleteNode();
	cout << "Printing after deletion 4, like dequeuing " << endl;
	L1.Print();
	L1.Test();
	x = 7;
	L1.deleteNode(x);
	cout << "Printing after deletion 5, nothing should happen " << endl;
	L1.Print();
	L1.Test();
	L1.clear();
	cout << "Printing after deletion 6, empty list " << endl;
	L1.Print();
	L1.Test();
	L1.deleteNode();
	cout << "Printing after deletion 7, nothing should happen " << endl;
	L1.Print();
	L1.Test();

	system("pause");
	return 0;
}