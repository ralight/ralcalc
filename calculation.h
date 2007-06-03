#ifndef CALCULATION_H
#define CALCULATION_H

#include "datatypes.h"

double doCalculation(double valueOne, double valueTwo, cToken lastToken);
double process(tokenItem **tokenList, const char *line);

#endif
