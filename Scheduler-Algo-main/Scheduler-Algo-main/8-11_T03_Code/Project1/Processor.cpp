#pragma once
//#include "Scheduler.h"
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
