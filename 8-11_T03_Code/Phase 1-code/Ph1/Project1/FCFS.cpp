#include "FCFS.h"

void FCFS::Loadp(ifstream& inputFile)
{
	inputFile >> MaxW;
}
void FCFS::Loadpf(ifstream& inputFile)
{
	inputFile >> forkprob;
}
void FCFS::Loadkill(ifstream& inputFile)
{
	while (inputFile.peek() != EOF)
	{
		int time;
		int pid;
		inputFile >> time;
		inputFile >> pid;
		SNode<int>* kill = new SNode <int>(time, pid);
		killSig.enqueue(kill);
	}
}

SQueue<int> FCFS::killSig;

char FCFS::Ptype = 'f';
double FCFS::forkprob = 0;
int FCFS::MaxW = 0;

