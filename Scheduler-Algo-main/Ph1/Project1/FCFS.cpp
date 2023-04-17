#include "FCFS.h"

void FCFS::Loadp(ifstream& inputFile)
{
	inputFile >> MaxW;
}
void FCFS::Loadpf(ifstream& inputFile)
{
	inputFile >> forkprob;
}
double FCFS::forkprob = 0;
int FCFS::MaxW = 0;