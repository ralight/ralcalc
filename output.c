#include <stdio.h>
#include <string.h>
#include <math.h>
#include "datatypes.h"


void printError(const char *line, int pos, errType error)
{
	int i;

	printf("\nError: %s\n       ", line);
	for(i = 0; i < pos; i++){
		printf(" ");
	}
	printf("^");
	switch(error){
		case errMemory:
			printf(" out of memory\n");
			break;
		case errBadInput:
			printf(" bad input to function (internal)\n");
		case errUnknownToken:
			printf(" unknown token\n");
			break;
		case errBadNumber:
			printf(" bad number\n");
			break;
		case errMismatchedBrackets:
			printf(" mismatched brackets\n");
			break;
		case errDuplicateNumber:
			printf(" duplicate number\n");
			break;
		case errInvalidOperator:
			printf(" invalid operator\n");
			break;
		case errInvalidBracket:
			printf(" invalid bracket\n");
			break;
		default:
			printf(" unknown error (%d)\n", error);
			break;
	}
}


void doubleToString(double value, char *string, int len)
{
	double absval;
	memset(string, 0, len);

	absval = fabs(value);

	if(absval >= 1000000000000000000000000.0){
		snprintf(string, len, "%gY", absval / 1000000000000000000000000.0);
	}else if(absval >= 1000000000000000000000.0){
		snprintf(string, len, "%gZ", absval / 1000000000000000000000.0);
	}else if(absval >= 1000000000000000000.0){
		snprintf(string, len, "%gE", absval / 1000000000000000000.0);
	}else if(absval >= 1000000000000000.0){
		snprintf(string, len, "%gP", absval / 1000000000000000.0);
	}else if(absval >= 1000000000000.0){
		snprintf(string, len, "%gT", absval / 1000000000000.0);
	}else if(absval >= 1000000000.0){
		snprintf(string, len, "%gG", absval / 1000000000.0);
	}else if(absval >= 1000000.0){
		snprintf(string, len, "%gM", absval / 1000000.0);
	}else if(absval >= 1000.0){
		snprintf(string, len, "%gk", absval / 1000.0);
	}else if(absval < 0.000000000000000000001){
		snprintf(string, len, "%gy", absval * 1000000000000000000000000.0);
	}else if(absval < 0.000000000000000001){
		snprintf(string, len, "%gz", absval * 1000000000000000000000.0);
	}else if(absval < 0.000000000000001){
		snprintf(string, len, "%ga", absval * 1000000000000000000.0);
	}else if(absval < 0.000000000001){
		snprintf(string, len, "%gf", absval * 1000000000000000.0);
	}else if(absval < 0.000000001){
		snprintf(string, len, "%gp", absval * 1000000000000.0);
	}else if(absval < 0.000001){
		snprintf(string, len, "%gn", absval * 1000000000.0);
	}else if(absval < 0.001){
		snprintf(string, len, "%gu", absval * 1000000.0);
	}else if(absval < 1.0){
		snprintf(string, len, "%gm", absval * 1000.0);
	}else{
		snprintf(string, len, "%g", absval);
	}
}


