#include "RR.h"


void RR::Loadp(ifstream& inputFile) {
    inputFile >> TS;
    inputFile >> RTF;

}

int RR::TS = 0;
int RR::RTF = 0;