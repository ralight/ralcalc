/*
 * Copyright (c) 2007, Roger Light
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "datatypes.h"


/*
 * Output an error with the line and a marker to where the
 * error is (probably).
 */
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


/*
 * Convert a double into a string with SI prefixes.
 * "string" is a preallocated char array of length len.
 */
void doubleToString(double value, char *string, int len)
{
	double absval;
	memset(string, 0, len);

	absval = fabs(value);

	if(absval >= 1000000000000000000000000.0){
		snprintf(string, len, "%gY", value / 1000000000000000000000000.0);
	}else if(absval >= 1000000000000000000000.0){
		snprintf(string, len, "%gZ", value / 1000000000000000000000.0);
	}else if(absval >= 1000000000000000000.0){
		snprintf(string, len, "%gE", value / 1000000000000000000.0);
	}else if(absval >= 1000000000000000.0){
		snprintf(string, len, "%gP", value / 1000000000000000.0);
	}else if(absval >= 1000000000000.0){
		snprintf(string, len, "%gT", value / 1000000000000.0);
	}else if(absval >= 1000000000.0){
		snprintf(string, len, "%gG", value / 1000000000.0);
	}else if(absval >= 1000000.0){
		snprintf(string, len, "%gM", value / 1000000.0);
	}else if(absval >= 1000.0){
		snprintf(string, len, "%gk", value / 1000.0);
	}else if(absval < 0.00000000000000000001){
		snprintf(string, len, "%gy", value * 1000000000000000000000000.0);
	}else if(absval < 0.00000000000000001){
		snprintf(string, len, "%gz", value * 1000000000000000000000.0);
	}else if(absval < 0.00000000000001){
		snprintf(string, len, "%ga", value * 1000000000000000000.0);
	}else if(absval < 0.00000000001){
		snprintf(string, len, "%gf", value * 1000000000000000.0);
	}else if(absval < 0.00000001){
		snprintf(string, len, "%gp", value * 1000000000000.0);
	}else if(absval < 0.00001){
		snprintf(string, len, "%gn", value * 1000000000.0);
	}else if(absval < 0.01){
		snprintf(string, len, "%gu", value * 1000000.0);
	}else if(absval < 0.1){
		snprintf(string, len, "%gm", value * 1000.0);
	}else{
		snprintf(string, len, "%g", value);
	}
}


