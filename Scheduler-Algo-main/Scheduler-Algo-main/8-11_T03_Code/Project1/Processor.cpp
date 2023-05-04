#include "Scheduler.h"
Processor:: Processor()
{
    CurrRun = nullptr;
    RDY_Length = 0;
    BusyCount = 0;
    Total_CPUtime = 0;
    Total_TRT = 0;
    //sc = new  Scheduler;
}
void Processor ::setRDY_Length(int length)
{
    RDY_Length = length;
}