/*
 * File: output.c Author: Roger Light
 * Project: ralcalc
 * Desc: Format output numbers and provide error messages.
 *
 * Copyright (C) 2007,2008 Roger Light.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <libintl.h>
#include <math.h>

#include "config.h"
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

	fprintf(stderr, "\nError: %s\n       ", line);
	for(i = 0; i < pos; i++){
		fprintf(stderr, " ");
	}
	fprintf(stderr, "^");
	switch(error){
		case errMemory:
			fprintf(stderr, _(" out of memory\n"));
			break;
		case errBadInput:
			fprintf(stderr, _(" bad input to function (internal)\n"));
		case errUnknownToken:
			fprintf(stderr, _(" unknown token\n"));
			break;
		case errBadNumber:
			fprintf(stderr, _(" bad number\n"));
			break;
		case errMismatchedBrackets:
			fprintf(stderr, _(" mismatched brackets\n"));
			break;
		case errDuplicateNumber:
			fprintf(stderr, _(" duplicate number\n"));
			break;
		case errInvalidOperator:
			fprintf(stderr, _(" invalid operator\n"));
			break;
		case errInvalidBracket:
			fprintf(stderr, _(" invalid bracket\n"));
			break;
		case errBadFile:
			fprintf(stderr, _(" unable to open file\n"));
		default:
			fprintf(stderr, _(" unknown error (%d)\n"), error);
			break;
	}
}


