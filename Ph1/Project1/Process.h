#pragma once
#include "StructQueue.h"
#include <iostream>
using namespace std;

class Process
{
private:
    int PID;
    int ArrivalTime;
    int ResponseTime;
    int CPUtime;
    int TerminationTime;
    int TurnaroundDuration;
    int waitingTime;
   SNode <int>* IOpairs;
    SQueue<int> IO_queue;
    int timeRemaining;
    int No_of_IO; //no of times user request input or output
    bool isBlocked;
    bool isFinished;
    Process* LCH;
    Process* RCH;
    //int lch_ID;   //1st child ID set to -1 if no children added by Amira
    //int rch_ID;   //1st child ID set to -1 if no children added by Amira
    //int ch_count; //number of children; set to 0 by default added by Amira
    
public:
    Process(int AT, int ID, int CT, SQueue<int> N); 
   void load(const string& filename);
    int getPID();
    int getArrivalTime() ;
    int getResponseTime() ;
    void settimeRemaining(int time);
    int gettimeRemaining();
    void setResponseTime(int time);
    int getTerminationTime() ;
    void setTerminationTime(int time);
    int getTurnaroundDuration() ;
    int getWaitingTime() ;
    bool getisBlocked();
    void setisBlocked(bool it);
    void setisFinished(bool it);
    bool getisFinished();
    void execute(int currentTimeStep);
    //void requestIO(int currentTimeStep, int inputRequestTime);
    //void completeIO(int currentTimeStep);
 //   friend ostream& operator<<(ostream& output,  Process* p1);
    ~Process();    
};
