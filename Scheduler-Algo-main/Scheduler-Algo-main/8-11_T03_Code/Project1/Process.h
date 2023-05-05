#pragma once
#include "StructQueue.h"
#include <iostream>
#include <fstream>
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
    bool orphanflag;
    Process* LCH;
    Process* RCH;
    //int lch_ID;   //1st child ID set to -1 if no children added by Amira
    //int rch_ID;   //1st child ID set to -1 if no children added by Amira
    //int ch_count; //number of children; set to 0 by default added by Amira
    
public:
 Process();
    Process(int AT, int ID, int CT,int Num, SQueue<int> N); 
   Process * load(ifstream& inputFile);
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
    bool getorphanflag();
    void setisBlocked(bool it);
    void setisFinished(bool it);
    void setorphanflag();
    bool getisFinished();
    void execute(int currentTimeStep);
    SQueue<int> getIOqueue();
    ///////////// Added by Amira /////////////
    Process(int AT, int ID, int CT); //overloaded constructor for forking
    void setRCH(Process* p);
    void setLCH(Process* p);
    Process* getRCH();
    Process* getLCH();
    int getCpuTime();
    ////////////////////////////////////////
     //Process newcreation(int AT, int ID, int CT, SQueue<int> N);
    //void requestIO(int currentTimeStep, int inputRequestTime);
    //void completeIO(int currentTimeStep);
  friend ostream& operator<<(ostream& output,  Process* p1);
    ~Process();    
};
