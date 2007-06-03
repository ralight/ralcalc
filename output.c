#include <stdio.h>
#include <string.h>
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
	memset(string, 0, len);

	if(value >= 1000000000000000000000000.0){
		snprintf(string, len, "%gY", value / 1000000000000000000000000.0);
	}else if(value >= 1000000000000000000000.0){
		snprintf(string, len, "%gZ", value / 1000000000000000000000.0);
	}else if(value >= 1000000000000000000.0){
		snprintf(string, len, "%gE", value / 1000000000000000000.0);
	}else if(value >= 1000000000000000.0){
		snprintf(string, len, "%gP", value / 1000000000000000.0);
	}else if(value >= 1000000000000.0){
		snprintf(string, len, "%gT", value / 1000000000000.0);
	}else if(value >= 1000000000.0){
		snprintf(string, len, "%gG", value / 1000000000.0);
	}else if(value >= 1000000.0){
		snprintf(string, len, "%gM", value / 1000000.0);
	}else if(value >= 1000.0){
		snprintf(string, len, "%gk", value / 1000.0);
	}else if(value < 0.000000000000000000001){
		snprintf(string, len, "%gy", value * 1000000000000000000000000.0);
	}else if(value < 0.000000000000000001){
		snprintf(string, len, "%gz", value * 1000000000000000000000.0);
	}else if(value < 0.000000000000001){
		snprintf(string, len, "%ga", value * 1000000000000000000.0);
	}else if(value < 0.000000000001){
		snprintf(string, len, "%gf", value * 1000000000000000.0);
	}else if(value < 0.000000001){
		snprintf(string, len, "%gp", value * 1000000000000.0);
	}else if(value < 0.000001){
		snprintf(string, len, "%gn", value * 1000000000.0);
	}else if(value < 0.001){
		snprintf(string, len, "%gu", value * 1000000.0);
	}else if(value < 1.0){
		snprintf(string, len, "%gm", value * 1000.0);
	}else{
		snprintf(string, len, "%g", value);
	}
}


