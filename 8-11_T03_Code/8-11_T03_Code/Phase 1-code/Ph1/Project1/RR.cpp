#include "RR.h"

void RR::Loadp(ifstream& inputFile) {
    inputFile >> TS;
    inputFile >> RTF;

}
char RR::Ptype = 'r';

int RR::TS = 0;
int RR::RTF = 0;