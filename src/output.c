/*
 * File: output.c Author: Roger Light
 * Project: ralcalc
 * Desc: Format output numbers and provide error messages.
 *
 * Copyright (C) 2007-2009 Roger Light.
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
#include "output.h"


/*
 * doubleToString()
 *
 * Convert a double into a string with SI prefixes.
 * "string" is a preallocated char array of length len.
 */
void doubleToString(double value, char *string, int len, char siPrefix, int precision)
{
	double absval;
	char strFormat[20];

	memset(string, 0, len);

	absval = fabs(value);

	if((absval >= 1.0e30 && siPrefix == '\0') || siPrefix == 'Q'){
		snprintf(string, len, "%gQ", value / 1.0e30);
	}else if((absval >= 1.0e27 && siPrefix == '\0') || siPrefix == 'R'){
		snprintf(string, len, "%gR", value / 1.0e27);
	}else if((absval >= 1.0e24 && siPrefix == '\0') || siPrefix == 'Y'){
		snprintf(string, len, "%gY", value / 1.0e24);
	}else if((absval >= 1.0e21 && siPrefix == '\0') || siPrefix == 'Z'){
		snprintf(string, len, "%gZ", value / 1.0e21);
	}else if((absval >= 1.0e18 && siPrefix == '\0') || siPrefix == 'E'){
		snprintf(string, len, "%gE", value / 1.0e18);
	}else if((absval >= 1.0e15 && siPrefix == '\0') || siPrefix == 'P'){
		snprintf(string, len, "%gP", value / 1.0e15);
	}else if((absval >= 1.0e12 && siPrefix == '\0') || siPrefix == 'T'){
		snprintf(string, len, "%gT", value / 1.0e12);
	}else if((absval >= 1.0e9 && siPrefix == '\0') || siPrefix == 'G'){
		snprintf(string, len, "%gG", value / 1.0e9);
	}else if((absval >= 1.0e6 && siPrefix == '\0') || siPrefix == 'M'){
		snprintf(string, len, "%gM", value / 1.0e6);
	}else if((absval >= 1.0e3 && siPrefix == '\0') || siPrefix == 'k'){
		snprintf(string, len, "%gk", value / 1.0e3);
	}else if((absval < 1.0e-27 && siPrefix == '\0') || siPrefix == 'q'){
		snprintf(string, len, "%gq", value * 1.0e30);
	}else if((absval < 1.0e-24 && siPrefix == '\0') || siPrefix == 'r'){
		snprintf(string, len, "%gr", value * 1.0e27);
	}else if((absval < 1.0e-21 && siPrefix == '\0') || siPrefix == 'y'){
		snprintf(string, len, "%gy", value * 1.0e24);
	}else if((absval < 1.0e-18 && siPrefix == '\0') || siPrefix == 'z'){
		snprintf(string, len, "%gz", value * 1.0e21);
	}else if((absval < 1.0e-15 && siPrefix == '\0') || siPrefix == 'a'){
		snprintf(string, len, "%ga", value * 1.0e18);
	}else if((absval < 1.0e-12 && siPrefix == '\0') || siPrefix == 'f'){
		snprintf(string, len, "%gf", value * 1.0e15);
	}else if((absval < 1.0e-9 && siPrefix == '\0') || siPrefix == 'p'){
		snprintf(string, len, "%gp", value * 1.0e12);
	}else if((absval < 1.0e-6 && siPrefix == '\0') || siPrefix == 'n'){
		snprintf(string, len, "%gn", value * 1.0e9);
	}else if((absval < 1.0e-3 && siPrefix == '\0') || siPrefix == 'u'){
		snprintf(string, len, "%gu", value * 1.0e6);
	}else if((absval < 0.1 && siPrefix == '\0') || siPrefix == 'm'){
		snprintf(string, len, "%gm", value * 1.0e3);
	}else{
		if(precision == -1){
			snprintf(string, len, "%g", value);
		}else{
			snprintf(strFormat, 20, "%%.%dg", precision);
			snprintf(string, len, strFormat, value);
		}
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
			break;
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
			break;
		default:
			fprintf(stderr, _(" unknown error (%d)\n"), error);
			break;
	}
}
