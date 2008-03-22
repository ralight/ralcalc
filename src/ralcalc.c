/*
 * File: ralcalc.c Author: Roger Light
 * Project: ralcalc
 * Desc: Main program.
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
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <arpa/inet.h>
#include <libintl.h>

#include "config.h"
#include "calculation.h"
#include "datatypes.h"
#include "tokens.h"
#include "output.h"

typedef enum {
	dmSI,
	dmExponent,
	dmRaw
} displayMode;

void print_usage()
{
	printf(_("ralcalc  version %s (build date: %s)\n"), VERSION, BUILDDATE);
	printf(_("Copyright (C) 2007,2008 Roger Light\nhttp://atchoo.org/tools/ralcalc/\n\n"));
	printf(_("ralcalc comes with ABSOLUTELY NO WARRANTY.  You may distribute ralcalc freely\nas described in the COPYING file distributed with this program.\n\n"));
	printf(_("ralcalc is a simple command line calculator. \n\n"));
	printf(_("Usage: ralcalc -h   (display this text)\n"));
	printf(_("       ralcalc [-q] [-e] [-r] <an equation>\n\n"));
	printf(_("Options\n"));
	printf(_(" -q	   Only display the answer (quiet).\n"));
	printf(_(" -e	   Use the '1e-3' form of display for the answer rather than SI prefixes.\n"));
	printf(_(" -r    Display the answer with neither SI prefixes nor exponents.\n"));
	printf(_("\nSee http://atchoo.org/tools/ralcalc/ for updates.\n"));
}


int main(int argc, char *argv[])
{
	int i, j, k;
	char *line = NULL;
	int hasError = 0;
	int len = 1;
	int rc;
	tokenItem tokenList;
	tokenItem *thisItem;
	int errorPos;
	char resultStr[100];
	double result;
	double lastResult = 0.0;
	char rcpath[100];
	FILE *rcptr;
	int quiet = 0;
	displayMode dm = dmSI;
	
	setlocale(LC_ALL, "");
	bindtextdomain("ralcalc", LOCALEDIR);
	textdomain("ralcalc");

	if(argc==2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))){
		print_usage();
		return 0;
	}

	for(i = 1; i < argc; i++){
		if(!strcmp(argv[i], "-q")){
			quiet = 1;
			argv[i][0] = '\0';
		}else if(!strcmp(argv[i], "-e")){
			dm = dmExponent;
			argv[i][0] = '\0';
		}else if(!strcmp(argv[i], "-r")){
			dm = dmRaw;
			argv[i][0] = '\0';
		}
	}

	for(i = 1; i < argc; i++){
		len += strlen(argv[i]);
	}

	if(getenv("HOME")){
		snprintf(rcpath, 100, "%s/.ralcalc_result", getenv("HOME"));

		rcptr = fopen(rcpath, "rb");
		if(rcptr){
			fread(&lastResult, sizeof(double), 1, rcptr);
			fclose(rcptr);
		}
	}

	line = calloc(len, sizeof(char));
	if(!line) return 1;

	i = 0;
	for(j = 1; j < argc; j++){
		for(k = 0; k < strlen(argv[j]); k++){
			if(argv[j][k] != ' '){
				line[i] = argv[j][k];
				i++;
			}
		}
	}

	/* First element always defined and not dynamic for less hassle */
	tokenList.next = NULL;
	tokenList.type = tkEndToken;

	rc = tokenise(&tokenList, line, lastResult, quiet);
	if(rc > 0) hasError = 1;

	rc = validate(&tokenList, line, quiet);
	if(rc > 0){
		hasError = 1;
		thisItem = tokenList.next;
		errorPos = 0;

		while(thisItem && rc > 1){
			errorPos += thisItem->length;
			thisItem = thisItem->next;
			rc--;
		}
	}

	rc = assignPrecedence(&tokenList);
	if(rc > 0) hasError = 1;

	if(!hasError && tokenList.next){
		result = process(&(tokenList.next));

		switch(dm){
			case dmSI:
				doubleToString(result, resultStr, 100);
				break;
			case dmExponent:
				snprintf(resultStr, 100, "%g", result);
				break;
			case dmRaw:
				snprintf(resultStr, 100, "%f", result);
				break;
		}

		if(!quiet){
			printf("%s = %s\n", line, resultStr);
		}else{
			printf("%s\n", resultStr);
		}

		if(getenv("HOME")){
			rcptr = fopen(rcpath, "wb");
			if(rcptr){
				fwrite(&result, sizeof(double), 1, rcptr);
				fclose(rcptr);
			}
		}
	}

	free(line);
	if(tokenList.next) freeList(tokenList.next);

	return hasError;
}

