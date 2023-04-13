#include "Process.h"


Process::Process(int AT, int ID, int CT, SQueue<int> N) : ArrivalTime(AT), PID(ID), CPUtime(CT), IO_queue(N)
{
    timeRemaining = CT;
    isBlocked = false;
    isFinished = false;
    ResponseTime = -1;
    TerminationTime = -1;
    waitingTime = 0;
    TurnaroundDuration = 0;
    //No_of_IO = IO.size(); // no of times ha request input/output
    LCH = NULL;
    RCH = NULL;
    //lch_ID = -1;  //1st child ID set to -1 if no children added by Amira
    //rch_ID = -1;   //1st child ID set to -1 if no children added by Amira
    //ch_count = 0; //number of children; set to 0 by default added by Amira
}

int Process::getPID() 
{
    return PID;
}

int Process::getArrivalTime() 
{
    return ArrivalTime;
}

int Process::getResponseTime() 
{
    return ResponseTime;
}


void Process::settimeRemaining(int time)
{
    timeRemaining = time;
}

int Process::gettimeRemaining()
{
    return timeRemaining;
}


void Process::setResponseTime(int time) 
{
    ResponseTime = time;
}

int Process::getTerminationTime() 
{
    return TerminationTime;
}

void Process::setTerminationTime(int time) 
{
    TerminationTime = time;
}

int Process::getTurnaroundDuration() 
{
    return TerminationTime - ArrivalTime;
}

int Process::getWaitingTime() 
{
    return waitingTime;
}

bool Process::getisBlocked() 
{
    return isBlocked;
}
void Process::setisBlocked(bool it)
{
    isBlocked =it;
}
void Process::setisFinished(bool it)
{
    isFinished = it;
}
bool Process::getisFinished()
{
    return isFinished;
}

void Process::execute(int currentTimeStep)  // decrement cpu at each time step 
{
    if (timeRemaining > 0) {
        timeRemaining--;
        if (timeRemaining == 0) 
        {
            isFinished = true;
            TerminationTime = currentTimeStep;
        }
    }
}
void Process::load(const string& filename)
{
    ifstream input(filename);
    if (!input.is_open())
    {
        cout << "Error opening file " << filename << endl;
        return;
    }
    string line;
    while (getline(input, line)) {
        istringstream iss(line);
        int AT, ID, CT, N;
        SQueue<int> IO_queue;

        if (!(iss >> AT >> ID >> CT >> N)) {
            std::cerr << "Error reading input data from file " << filename << std::endl;
            return;
        }

        for (int i = 0; i < N; i++)
        {
            int IO_R, IO_D;
            if (!(iss >> IO_R >> IO_D))
            {
                cout << "Error reading input data from file " << filename << std::endl;
                return;
            }
            IOpairs = new SNode<int>(IO_R, IO_D);
            IO_queue.enqueue({ IOpairs });
        }
    }
    input.close();
}

/*void Process::requestIO(int currentTimeStep, int inputRequestTime)
{
    isBlocked = true;
    this->inputRequestTime = inputRequestTime;
}

void Process::completeIO(int currentTimeStep) 
{
    isBlocked = false;
    int timeSpentWaiting = currentTimeStep - inputRequestTime;
    waitingTime += timeSpentWaiting;
    timeRemaining -= InputDuration;
}
*/

/*ostream& operator<<(ostream& output, Process* p1)
{
    output << "PID: " << p1->PID << "\n";
    output << "Arrival time: " << p1->ArrivalTime << "\n";
    output << "CPU time: " << p1->CPUtime << "\n";
    output << "Input/Output duration: " << p1->InputDuration << "\n";
    output << "Response time: " << p1->ResponseTime << "\n";
    output << "Termination time: " << p1->TerminationTime << "\n";
    output << "Turnaround duration: " << p1->getTurnaroundDuration() << "\n";
    output << "Waiting time: " << p1->waitingTime << "\n";
    output << "Blocked: " << (p1->getisBlocked() ? "Yes" : "No") << "\n";
    output << "Finished: " << (p1->getisFinished() ? "Yes" : "No") << "\n";
    return output;
}*/

Process::~Process(){}
