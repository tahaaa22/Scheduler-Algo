#pragma once
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
    int InputDuration;
    int inputRequestTime;
    int timeRemaining;
    int no; //no of times user request input or output
    bool isBlocked;
    bool isFinished;
    //int lch_ID;   //1st child ID set to -1 if no children added by Amira
    //int rch_ID;   //1st child ID set to -1 if no children added by Amira
    //int ch_count; //number of children; set to 0 by default added by Amira
    
public:
    Process(int id, int arrivalTime, int cpuTime, int inputDuration, int no); //ordinary process
    // parent only 
    int getPID();
    int getArrivalTime() ;
    void setArrivalTime();
    int getResponseTime() ;
    void setResponseTime(int time);
    int getTerminationTime() ;
    void setTerminationTime(int time);
    int getNo_ofinput(); //added by mimo
    void setno_ofinput(int time); //added by mimo
    int gettimeRemaining(); // added by mimo 
    void settimeRemaining(int time); // added by mimo
    int getTurnaroundDuration() ;
    int getWaitingTime() ;
    bool getisBlocked();
    void setisBlocked(bool it);
    void setisFinished(bool it);
    bool getisFinished();
    void execute(int currentTimeStep);
    void requestIO(int currentTimeStep, int inputRequestTime);
    void completeIO(int currentTimeStep);
 //   friend ostream& operator<<(ostream& output,  Process* p1);
    ~Process();    
};

