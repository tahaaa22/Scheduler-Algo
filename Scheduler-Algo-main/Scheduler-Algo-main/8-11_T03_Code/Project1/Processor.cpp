#pragma once
#include "Scheduler.h"
#include "Processor.h"
Processor:: Processor()
{
    CurrRun = nullptr;
    RDY_Length = 0;
    BusyCount = 0;
    Total_CPUtime = 0;
    TotalBusyTime = 0;
    TotalIdleTime = 0;
    type = 'p';
     OverheatTime=0;
     isOverheated = false;
}

double Processor :: getTotalBusy()
{
    return TotalBusyTime;
}

double Processor:: getTotalIDLE()
{
    return TotalIdleTime;
}

void Processor ::setisOverheated(bool b)
{
    isOverheated = b;
}

bool Processor :: getisOverheated()
{
    return isOverheated;
}

void Processor :: setOverheatTime(int n)
{
    OverheatTime = n;
}

int Processor :: getOverheatTime()
{
    return OverheatTime;
}

void Processor :: setRDY_Length(int length)
{
    RDY_Length = length;
}

void Processor :: setCurrRun(Process* p)
{
    CurrRun = p;
}

Process* Processor :: getCurrRun()
{
    return CurrRun;
}

int Processor :: getRDY_Length()
{
    return RDY_Length;
}

char Processor :: gettype()
{
    return type;
}

void Processor :: settype(char a)
{
    type = a;
}

bool Processor :: isBusy() // return true of currrun if working // dh feha 8lt need fix 
{
    if (CurrRun) return true;
    return false;
}

Processor :: ~Processor()
{

}