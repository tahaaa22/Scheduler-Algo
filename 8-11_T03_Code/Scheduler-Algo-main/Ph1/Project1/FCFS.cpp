#include "FCFS.h"


//ostream& operator<<(ostream& output, Processor* p1)
//{
//	output << p1->getRDYCount() << " ";
//	return output;
//}
void FCFS::Loadp(ifstream& inputFile)
{
	inputFile >> MaxW;
}
void FCFS::Loadpf(ifstream& inputFile)
{
	inputFile >> forkprob;
}

char FCFS::Ptype = 'f';

double FCFS::forkprob = 0;
int FCFS::MaxW = 0;