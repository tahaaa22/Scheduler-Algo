#pragma once
#include "StructQueue.h"
#include <iostream>
#include <fstream>
using namespace std;

class Process
{
private:
    int PID;
    bool iskilled;
    int totalIO_D; // added by taha for output file
    int firsttime; //to check response time - added by taha 
    int ArrivalTime;
    int ResponseTime;
    int CPUtime;
    int timeblk;
    int numIO;
    int TerminationTime;
    int TurnaroundDuration;
    int waitingTime;
    int deadline; // added by taha
    SNode <int>* IOpairs;
    SQueue<int> IO_queue;
    int timeRemaining;
    int No_of_IO; //no of times user request input or output
    bool isBlocked;
    bool isFinished;
    bool orphanflag; // added by taha
    Process* Parent;
    Process* LCH;
    Process* RCH;
    //int lch_ID;   //1st child ID set to -1 if no children added by Amira
    //int rch_ID;   //1st child ID set to -1 if no children added by Amira
    //int ch_count; //number of children; set to 0 by default added by Amira
    
public:
 Process();
 Process(int AT, int ID, int CT, int Num, SQueue<int> N, int d); //changed by taha
 Process(int AT, int ID, int CT, Process* parent); //overloaded constructor for forking
   Process * load(ifstream& inputFile);
   Process* getParent(); //getter for forked child to parent
   int getnumIO();
   void setiskilled(bool t);
   bool getiskilled();
   void setnumIO(int t);
   void setblktime(int t);
   int getblktime();
   void setwaitingtime(int t);
   void setTurnaroundDuration(int t);
   void setfirsttime(int t);
   int getfirsttime();
   int getTotalIO_D();
   void setTotalIO_D(int t);
    int getPID();
    bool getorphanflag();
    void setorphanflag(bool f);
    int getArrivalTime() ;
    int getResponseTime() ;
    void settimeRemaining(int time);
    int gettimeRemaining();
    void setResponseTime(int time);
    int getTerminationTime() ;
    void setTerminationTime(int time);
    int getTurnaroundDuration() ;
    int getWaitingTime() ;
    int getDeadLine(); //added by taha
    bool getisBlocked();
    void setisBlocked(bool it);
    void setisFinished(bool it);
    bool getisFinished();
    void execute(int currentTimeStep);
    SQueue<int> * getIOqueue();
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
