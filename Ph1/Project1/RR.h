#pragma once
#include "Processor.h"
#include "Queue.h"
class RR : public Processor
{
private:
    Queue<Process*> RdyQueue;
    int TS;    //Timeslice
    int RTF;
    int Curtime;
public:
    RR(int T=0)  
    {
        TS = T;
        Curtime = 0;
        //3yza agyb el rtf mn el load ba 
        loadp();
        settype('R');
    }
    virtual void addToReadyQueue(Process* process)
    {
        RdyQueue.enqueue(process);
    };
    void settimeslice(int t)
    {
        TS = t;
    };
    virtual void ScheduleAlgo(int currenttimestep)
    {
        if (!RdyQueue.isEmpty() && !getCurrRun()) {
            Process* temp;         
            RdyQueue.dequeue(temp);
            setCurrRun(temp);
        }

       /* if (Curtime == 0 && RdyQueue.getCount()>0)
        {
            Process* temp;
            RdyQueue.dequeue(temp);
            setCurrRun(temp);
            getCurrRun()->execute(currenttimestep);

        }
        else if (!getCurrRun())
        {
            if (Curtime + 1 == TS) // a5r sec
            {
                getCurrRun()->execute(currenttimestep);
                Curtime = 0;
                Process* temp= getCurrRun();
                int t = temp->gettimeRemaining();
                if (t > 0)
                {
                    RdyQueue.enqueue(temp);
                }
            }
            else
            {
                getCurrRun()->execute(currenttimestep);
                Curtime++;
            }
        }*/
            
            
            
       
    };
    bool CheckRTF(Process* p1);
    Process* getNextProcess() {
        if (RdyQueue.isEmpty()) return nullptr;
        else {
            Process* temp;
            RdyQueue.peek(temp);
            return temp;
        }
    }
    virtual void print_rdy()
    {
        RdyQueue.Print();
    }
    void Loadp() {

        // open the file
        ifstream inputFile("inputfile.txt");
        int int_number = 5; // RTF MAKANO 5
        int NO_RTF;
        int current_int_number = 0;

        if (inputFile.fail())
        {
            exit(1); // terminates program
        }
        else
        {
            while (inputFile >> NO_RTF)
            {
                current_int_number++;
                if (current_int_number == int_number) 
                {
                    RTF = NO_RTF;
                    break;
                }
            }
        }
        // close the file
        inputFile.close();
    }


};

