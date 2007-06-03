#ifndef OUTPUT_H
#define OUTPUT_H

#include "datatypes.h"

void printError(const char *line, int pos, errType error);
void doubleToString(double value, char *string, int len);

#endif
