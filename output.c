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
 * doubleToString()
 *
 * Convert a double into a string with SI prefixes.
 * "string" is a preallocated char array of length len.
 */
void doubleToString(double value, char *string, int len)
{
	double absval;
	memset(string, 0, len);

	absval = fabs(value);

	if(absval >= 1.0e24){
		snprintf(string, len, "%gY", value / 1.0e24);
	}else if(absval >= 1.0e21){
		snprintf(string, len, "%gZ", value / 1.0e21);
	}else if(absval >= 1.0e18){
		snprintf(string, len, "%gE", value / 1.0e18);
	}else if(absval >= 1.0e15){
		snprintf(string, len, "%gP", value / 1.0e15);
	}else if(absval >= 1.0e12){
		snprintf(string, len, "%gT", value / 1.0e12);
	}else if(absval >= 1.0e9){
		snprintf(string, len, "%gG", value / 1.0e9);
	}else if(absval >= 1.0e6){
		snprintf(string, len, "%gM", value / 1.0e6);
	}else if(absval >= 1.0e3){
		snprintf(string, len, "%gk", value / 1.0e3);
	}else if(absval < 1.0e-20){
		snprintf(string, len, "%gy", value * 1.0e24);
	}else if(absval < 1.0e-17){
		snprintf(string, len, "%gz", value * 1.0e21);
	}else if(absval < 1.0e-14){
		snprintf(string, len, "%ga", value * 1.0e18);
	}else if(absval < 1.0e-11){
		snprintf(string, len, "%gf", value * 1.0e15);
	}else if(absval < 1.0e-8){
		snprintf(string, len, "%gp", value * 1.0e12);
	}else if(absval < 1.0e-5){
		snprintf(string, len, "%gn", value * 1.0e9);
	}else if(absval < 1.0e-2){
		snprintf(string, len, "%gu", value * 1.0e6);
	}else if(absval < 0.1){
		snprintf(string, len, "%gm", value * 1.0e3);
	}else{
		snprintf(string, len, "%g", value);
	}
}


/*
 * printError()
 *
 * Output an error with the line and a marker to where the
 * error is (probably). This works when checking that the
 * characters and numbers are all valid, however has more
 * problems getting the correct marker position when the 
 * error involves incorrect token order.
 *
 * Arguments:
 * 		line - string containing the user input (no spaces).
 * 		pos - integer with the position of the error.
 * 		error - the error to report.
 */
void printError(const char *line, int pos, errType error, int quiet)
{
	int i;

	if(quiet) return;

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


