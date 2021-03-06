/*
 * File: ralcalc.c
 * Author: Roger Light
 * Project: ralcalc
 * Desc: Main program.
 *
 * Copyright (C) 2007-2009,2011,2013 Roger Light.
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


/* Path to where .ralcalc_results is */
static char *rcpath = NULL;

/* Internal functions */
void printUsage(void);
void printTokens(void);
int processLine(const char *line, int quiet, displayMode dm, char siPrefix, int precision);
int doFileInput(FILE *fptr, int quiet, displayMode dm, char siPrefix, int precision);
int doLineCalculation(int argc, char *argv[], int quiet, displayMode dm, char siPrefix, int precision);


void printUsage(void)
{
	printf(_("ralcalc  version %s (build date: %s)\n"), VERSION, BUILDDATE);
	printf(_("Copyright (C) 2007-2009,2011,2013 Roger Light\nhttp://atchoo.org/tools/ralcalc/\n\n"));
	printf(_("ralcalc comes with ABSOLUTELY NO WARRANTY.  You may distribute ralcalc freely\nas described in the COPYING file distributed with this program.\n\n"));
	printf(_("ralcalc is a simple command line calculator.\n\n"));
	printf(_("Usage: ralcalc -h   (display this text)\n"));
	printf(_("       ralcalc -a   (display all available operators)\n"));
	printf(_("       ralcalc [-e] [-f input file] [-i] [-p precision] [-q] [-r] [-s prefix] <an equation>\n"));
	printf(_("Options\n"));
	printf(_(" -e    Use the '1e-3' form of display for the answer rather than SI prefixes.\n"));
	printf(_(" -f    Process a list of calculations from a file.\n"));
	printf(_(" -i    Process a list of calculations from stdin.\n"));
	printf(_(" -p    Number of digits to display in the output when using -e or -r, or if the\n       output does not have an SI prefix.\n"));
	printf(_(" -q    Only display the answer (quiet).\n"));
	printf(_(" -r    Display the answer with neither SI prefixes nor exponents.\n"));
	printf(_(" -s    Use the SI prefix 'prefix' to format the output.\n"));
	printf(_("\nSee http://atchoo.org/tools/ralcalc/ for updates.\n"));
}


/*
 * printTokens()
 *
 * Print out all of the possible tokens and their description.
 */
void printTokens(void)
{
	printf(_("Numbers:\n"));
	printf(_("  1.0    : example number\n"));
	printf(_("  1.0e-3 : example number\n"));
	printf(_("  1.0m   : example number\n"));
	printf(_("  _      : previous result\n"));
	printf(_("  y, z, a, f, p, n, u, m : SI prefixes\n"));
	printf(_("  k, M, G, T, P, E, Z, Y : SI prefixes\n"));
	printf(_("\nOperators:\n"));
	printf(_("  +     : addition\n"));
	printf(_("  -     : subtraction\n"));
	printf(_("  * x   : multiplication\n"));
	printf(_("  /     : division\n"));
	printf(_("  %%     : modulus\n"));
	printf(_("  ^     : power\n"));
	printf(_("  () [] : brackets increase precedence\n"));
	printf(_("\nFunctions:\n"));
	printf(_("  log X         : logarithm to base 10 of X\n"));
	printf(_("  ln X          : natural logarithm of X\n"));
	printf(_("  sin X, asin X : sine, arcsin of X\n"));
	printf(_("  cos X, acos X : cosine, arccos of X\n"));
	printf(_("  tan X, atan X : tangent, arctan of X\n"));
	printf(_("  sqrt X        : square root of X\n"));
	printf(_("\nConstants:\n"));
	printf(_("  pi  : 3.14159\n"));
	printf(_("  exp : 2.71828\n"));
}


/*
 * processLine()
 *
 * Take a string, tokenise it, validate it and pass it on for calculation. This
 * is the glue function of ralcalc really.
 */
int processLine(const char *line, int quiet, displayMode dm, char siPrefix, int precision)
{
	tokenItem tokenList;
	int rc;
	FILE *rcptr;
	double result;
	double lastResult = 0.0;
	int hasError = 0;
	char resultStr[100];
	char formatStr[20];

	if(!line) return errBadInput;

	if(rcpath){
		rcptr = fopen(rcpath, "rb");
		if(rcptr){
			rc = fread(&lastResult, sizeof(double), 1, rcptr);
			if(rc != 1){
				fprintf(stderr, "Warning: Previous value file corrupt, ignoring.\n");
				lastResult = 0.0;
			}
			fclose(rcptr);
		}
	}

	/* First element always defined and not dynamic for less hassle */
	tokenList.next = NULL;
	tokenList.type = tkEndToken;

	rc = tokenise(&tokenList, line, lastResult, quiet);
	if(rc != errNoError) hasError = 1;

	rc = validate(&tokenList, line, quiet);
	if(rc != errNoError) hasError = 1;

	rc = assignPrecedence(&tokenList);
	if(rc != errNoError) hasError = 1;

	if(!hasError && tokenList.next){
		result = process(&(tokenList.next));

		switch(dm){
			case dmSI:
				doubleToString(result, resultStr, 100, siPrefix, precision);
				break;
			case dmExponent:
				if(precision == -1){
					snprintf(resultStr, 100, "%lg", result);
				}else{
					snprintf(formatStr, 20, "%%.%dlg", precision);
					snprintf(resultStr, 100, formatStr, result);
				}
				break;
			case dmRaw:
				if(precision == -1){
					snprintf(resultStr, 100, "%f", result);
				}else{
					snprintf(formatStr, 20, "%%.%df", precision);
					snprintf(resultStr, 100, formatStr, result);
				}
				break;
		}

		if(!quiet){
			printf("%s = %s\n", line, resultStr);
		}else{
			printf("%s\n", resultStr);
		}

		if(rcpath){
			rcptr = fopen(rcpath, "wb");
			if(rcptr){
				rc = fwrite(&result, sizeof(double), 1, rcptr);
				if(rc != 1){
					fprintf(stderr, "Error writing last value file.\n");
				}
				fclose(rcptr);
			}
		}
	}

	if(tokenList.next) freeList(tokenList.next);

	return hasError;
}


/*
 * doFileInput()
 *
 * Get lines from a file (be that a file on disk or stdin) and pass the lines
 * to processLine() for calculation.
 */
int doFileInput(FILE *fptr, int quiet, displayMode dm, char siPrefix, int precision)
{
	char *line;
	int rc = errNoError;

	if(!fptr) return errBadInput;

	line = calloc(1024, sizeof(char));
	if(!line){
		fprintf(stderr, _("Error: Out of memory\n"));
		return errMemory;
	}
	fgets(line, 1024, fptr);
	while(!feof(fptr)){
		if(line[strlen(line)-1] == 10 || line[strlen(line)-1] == 13){
			line[strlen(line)-1] = '\0';
		}
		/* Quit if "q" (or "quit" etc.) is an input */
		if(line[0] == 'q' || line[0] == 'Q'){
			rc = errNoError;
			break;
		}
		rc = processLine(line, quiet, dm, siPrefix, precision);
		if(rc != errNoError){
			break;
		}
		fgets(line, 1024, fptr);
	}
	free(line);

	return rc;
}


/*
 * doLineCalculation()
 *
 * Convert argc and argv into a single string and pass them to processLine for
 * calculation.
 */
int doLineCalculation(int argc, char *argv[], int quiet, displayMode dm, char siPrefix, int precision)
{
	char *line;
	int i, j;
	unsigned int k;
	int len = 0;
	int rc = errNoError;

	for(i = 1; i < argc; i++){
		len += strlen(argv[i]) + 1;
	}

	line = calloc(len + 1, sizeof(char));
	if(!line){
		fprintf(stderr, _("Error: Out of memory\n"));
		return errMemory;
	}

	i = 0;
	for(j = 1; j < argc; j++){
		if(strlen(argv[j]) > 0){
			for(k = 0; k < strlen(argv[j]); k++){
				line[i] = argv[j][k];
				i++;
			}
			if(j < argc - 1){
				line[i] = ' ';
				i++;
			}
		}
	}
	rc = processLine(line, quiet, dm, siPrefix, precision);
	free(line);

	return rc;
}


int main(int argc, char *argv[])
{
	int i;
	int quiet = 0;
	displayMode dm = dmSI;
	int rc = 0;
	char *ifile = NULL;
	FILE *iptr;
	FILE *rcptr;
	int usestdin = 0;
	char siPrefix = '\0';
	char *home = getenv("HOME");
	int rcpathlen;
	int precision = -1;
	char *token;
	char line[200];
	
	setlocale(LC_ALL, "");
	bindtextdomain("ralcalc", LOCALEDIR);
	textdomain("ralcalc");

	if(argc==2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))){
		printUsage();
		return 1;
	}

	if(argc==2 && (!strcmp(argv[1], "-a") || !strcmp(argv[1], "--all"))){
		printTokens();
		return 1;
	}

	if(home){
		rcpathlen = strlen(home) + strlen("/.ralcalcrc") + 1;
		rcpath = (char *)malloc(rcpathlen * sizeof(char));
		snprintf(rcpath, rcpathlen, "%s/.ralcalcrc", home);
		rcptr = fopen(rcpath, "rb");
		if(rcptr){
			if(fgets(line, 200, rcptr)){
				while(line[strlen(line)-1] == '\n'){
					line[strlen(line)-1] = '\0';
				}
				token = strtok(line, " ");
				if(!strcmp(line, "-e")){
					dm = dmExponent;
				}else if(!strcmp(line, "-r")){
					dm = dmRaw;
				}else if(!strcmp(line, "-s")){
					dm = dmSI;
					token = strtok(NULL, " ");
					siPrefix = token[0];
				}
			}
			fclose(rcptr);
		}
	}

	for(i = 1; i < argc; i++){
		if(!strcmp(argv[i], "-q")){
			quiet = 1;
			argv[i][0] = '\0';
		}else if(!strcmp(argv[i], "-e")){
			dm = dmExponent;
			argv[i][0] = '\0';
		}else if(!strcmp(argv[i], "-f")){
			if(i < argc - 1){
				ifile = strdup(argv[i+1]);
				argv[i][0] = '\0';
				argv[i+1][0] = '\0';
				i++;
			}else{
				printUsage();
				return 1;
			}
		}else if(!strcmp(argv[i], "-i")){
			usestdin = 1;
		}else if(!strcmp(argv[i], "-p")){
			if(i < argc - 1){
				precision = atoi(argv[i+1]);
				argv[i][0] = '\0';
				argv[i+1][0] = '\0';
				i++;
				if(precision < 0){
					fprintf(stderr, _("Error: Precision must be a positive integer.\n"));
					return 1;
				}
			}else{
				printUsage();
				return 1;
			}
		}else if(!strcmp(argv[i], "-r")){
			dm = dmRaw;
			argv[i][0] = '\0';
		}else if(!strcmp(argv[i], "-s")){
			if(i < argc - 1){
				if(strlen(argv[i+1]) != 1){
					fprintf(stderr, _("Error: Invalid SI prefix '%s' for '-s' option.\n"), argv[i+1]);
					return 1;
				}else{
					dm = dmSI;
					switch(argv[i+1][0]){
						case 'Y':
						case 'Z':
						case 'E':
						case 'P':
						case 'T':
						case 'G':
						case 'M':
						case 'k':
						case 'm':
						case 'u':
						case 'n':
						case 'p':
						case 'f':
						case 'a':
						case 'z':
						case 'y':
							siPrefix = argv[i+1][0];
							break;
						default:
							fprintf(stderr, _("Error: Invalid SI prefix '%s' for '-s' option.\n"), argv[i+1]);
							return 1;
							break;
					}
					argv[i][0] = '\0';
					argv[i+1][0] = '\0';
					i++;
				}
			}else{
				printUsage();
				return 1;
			}
		}
	}

	/* Figure out the path to .ralcalc_result for loading/saving. */
	if(home){
		rcpathlen = strlen(home) + strlen("/.ralcalc_result") + 1;
		rcpath = (char *)malloc(rcpathlen * sizeof(char));
		snprintf(rcpath, rcpathlen, "%s/.ralcalc_result", home);
	}

	/* Do calculation based on input arguments first */
	if(doLineCalculation(argc, argv, quiet, dm, siPrefix, precision)) rc = 1;
	
	/* Do calculations from a disk file */
	if(ifile){
		iptr = fopen(ifile, "rt");
		if(!iptr){
			fprintf(stderr, _("Error: Unable to open file \"%s\"\n"), ifile);
			return 1;
		}
		if(doFileInput(iptr, quiet, dm, siPrefix, precision)) rc = 1;
		fclose(iptr);
	}

	/* Read calculations from stdin */
	if(usestdin){
		if(doFileInput(stdin, quiet, dm, siPrefix, precision)) rc = 1;
	}

	if(rcpath){
		free(rcpath);
	}

	return rc;
}

