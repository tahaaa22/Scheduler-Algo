#pragma once
//#include "Scheduler.h"
#include "Processor.h"
Processor:: Processor()
{
    CurrRun = nullptr;
    RDY_Length = 0;
    BusyCount = 0;
    Total_CPUtime = 0;
    Total_TRT = 0;
}
