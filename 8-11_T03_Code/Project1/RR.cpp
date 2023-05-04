#include "RR.h"


//ostream& operator<<(ostream& output, Processor* p1)
//{
//    output << p1->getRDYCount() << " ";
//    return output;
//}

void RR::Loadp(ifstream& inputFile) {
    inputFile >> TS;
    inputFile >> RTF;

}
char RR::Ptype = 'r';

int RR::TS = 0;
int RR::RTF = 0;