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

struct ralcalc_config {
	int quiet;
	displayMode dm;
	int rc;
	char *ifile;
	FILE *iptr;
	FILE *rcptr;
	int usestdin;
	char siPrefix;
	char *config_dir;
	char *data_dir;
	int precision;
};


/* Internal functions */
void printUsage(void);
void printTokens(void);
int processLine(const char *line, struct ralcalc_config *config);
int doFileInput(FILE *fptr, struct ralcalc_config *config);
int doLineCalculation(int argc, char *argv[], struct ralcalc_config *config);
void readLastResult(double *value);
void writeLastResult(double value);


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
int processLine(const char *line, struct ralcalc_config *config)
{
	tokenItem tokenList;
	int rc;
	double result;
	double lastResult = 0.0;
	int hasError = 0;
	char resultStr[100];
	char formatStr[20];

	if(!line) return errBadInput;

	readLastResult(&lastResult);

	/* First element always defined and not dynamic for less hassle */
	tokenList.next = NULL;
	tokenList.type = tkEndToken;

	rc = tokenise(&tokenList, line, lastResult, config->quiet);
	if(rc != errNoError) hasError = 1;

	rc = validate(&tokenList, line, config->quiet);
	if(rc != errNoError) hasError = 1;

	rc = assignPrecedence(&tokenList);
	if(rc != errNoError) hasError = 1;

	if(!hasError && tokenList.next){
		result = process(&(tokenList.next));

		switch(config->dm){
			case dmSI:
				doubleToString(result, resultStr, 100, config->siPrefix, config->precision);
				break;
			case dmExponent:
				if(config->precision == -1){
					snprintf(resultStr, 100, "%lg", result);
				}else{
					snprintf(formatStr, 20, "%%.%dlg", config->precision);
					snprintf(resultStr, 100, formatStr, result);
				}
				break;
			case dmRaw:
				if(config->precision == -1){
					snprintf(resultStr, 100, "%f", result);
				}else{
					snprintf(formatStr, 20, "%%.%df", config->precision);
					snprintf(resultStr, 100, formatStr, result);
				}
				break;
		}

		if(!config->quiet){
			printf("%s = %s\n", line, resultStr);
		}else{
			printf("%s\n", resultStr);
		}

		writeLastResult(result);
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
int doFileInput(FILE *fptr, struct ralcalc_config *config)
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
		rc = processLine(line, config);
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
int doLineCalculation(int argc, char *argv[], struct ralcalc_config *config)
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
	rc = processLine(line, config);
	free(line);

	return rc;
}


int loadConfigPath(struct ralcalc_config *config, const char *path)
{
	FILE *rcptr;
	char *token;
	char line[200];

	rcptr = fopen(path, "rb");
	if(rcptr){
		if(fgets(line, 200, rcptr)){
			while(line[strlen(line)-1] == '\n'){
				line[strlen(line)-1] = '\0';
			}
			token = strtok(line, " ");
			if(!strcmp(line, "-e")){
				config->dm = dmExponent;
			}else if(!strcmp(line, "-r")){
				config->dm = dmRaw;
			}else if(!strcmp(line, "-s")){
				config->dm = dmSI;
				token = strtok(NULL, " ");
				config->siPrefix = token[0];
			}
		}
		fclose(rcptr);
	}else{
		return 1;
	}

	return 0;
}


/* Load configuration file
 *
 * Either:
 *
 * $XDG_CONFIG_HOME/ralcalcrc
 * $HOME/.config/ralcalcrc
 * $HOME/.ralcalcrc
 */
int load_config(struct ralcalc_config *config)
{
	char *xdg_config;
	char *home;
	char *path;
	int pathlen;
	int rc;

	xdg_config = getenv("XDG_CONFIG_HOME");

	if(xdg_config){
		pathlen = strlen(xdg_config) + strlen("/ralcalcrc") + 1;
		path = malloc(pathlen);
		snprintf(path, pathlen, "%s/ralcalcrc", xdg_config);
		if(!path){
			fprintf(stderr, "Error: Out of memory.\n");
			return 1;
		}
		rc = loadConfigPath(config, path);
		free(path);
		return rc;
	}else{
		home = getenv("HOME");
		if(home){
			pathlen = strlen(home) + strlen("/.config/ralcalcrc") + 1;
			path = (char *)malloc(pathlen * sizeof(char));
			if(!path){
				fprintf(stderr, "Error: Out of memory.\n");
				return 1;
			}
			snprintf(path, pathlen, "%s/.config/ralcalcrc", home);

			rc = loadConfigPath(config, path);
			free(path);

			if(!rc) return 0;

			/* Loading from $HOME/.config/ralcalcrc failed, try old location. */
			pathlen = strlen(home) + strlen("/.ralcalcrc") + 1;
			path = (char *)malloc(pathlen * sizeof(char));
			if(!path){
				fprintf(stderr, "Error: Out of memory.\n");
				return 1;
			}
			snprintf(path, pathlen, "%s/.ralcalcrc", home);

			rc = loadConfigPath(config, path);
			free(path);

			return rc;
		}else{
			/* No config file loaded, but carry on anyway */
			return 0;
		}
	}
}


int writeLastResultPath(double value, const char *path)
{
	int rc = 0;
	FILE *fptr;

	fptr = fopen(path, "wb");
	if(fptr){
		if(fwrite(&value, sizeof(double), 1, fptr) != 1){
			fprintf(stderr, "Error writing last value file.\n");
			rc = 1;
		}
		fclose(fptr);
	}
	return rc;
}


void writeLastResult(double value)
{
	char *xdg_data;
	char *home;
	char *path;
	int pathlen;
	int rc;

	xdg_data = getenv("XDG_DATA_HOME");

	if(xdg_data){
		pathlen = strlen(xdg_data) + strlen("/ralcalc_result") + 1;
		path = malloc(pathlen);
		snprintf(path, pathlen, "%s/ralcalc_result", xdg_data);
		if(!path){
			fprintf(stderr, "Error: Out of memory.\n");
			return;
		}
		rc = writeLastResultPath(value, path);
		free(path);
		return;
	}else{
		home = getenv("HOME");
		if(home){
			pathlen = strlen(home) + strlen("/.local/share/ralcalc_result") + 1;
			path = (char *)malloc(pathlen * sizeof(char));
			if(!path){
				fprintf(stderr, "Error: Out of memory.\n");
				return;
			}
			snprintf(path, pathlen, "%s/.local/share/ralcalc_result", home);

			rc = writeLastResultPath(value, path);
			free(path);

			if(!rc) return;

			/* Loading from $HOME/.local/share/ralcalc_result failed, try old location. */
			pathlen = strlen(home) + strlen("/.ralcalc_result") + 1;
			path = (char *)malloc(pathlen * sizeof(char));
			if(!path){
				fprintf(stderr, "Error: Out of memory.\n");
				return;
			}
			snprintf(path, pathlen, "%s/.ralcalc_result", home);

			rc = writeLastResultPath(value, path);
			free(path);
		}
	}

}


int readLastResultPath(double *value, const char *path)
{
	FILE *fptr;

	fptr = fopen(path, "rb");
	if(fptr){
		int rc = fread(value, sizeof(double), 1, fptr);
		fclose(fptr);
		if(rc == 1){
			return 0;
		}else{
			fprintf(stderr, "Warning: Previous value file corrupt, ignoring.\n");
			*value = 0.0;
			return 1;
		}
	}

	return 1;
}


void readLastResult(double *value)
{
	char *xdg_data;
	char *home;
	char *path;
	int pathlen;
	int rc;

	xdg_data = getenv("XDG_DATA_HOME");

	if(xdg_data){
		pathlen = strlen(xdg_data) + strlen("/ralcalc_result") + 1;
		path = malloc(pathlen);
		snprintf(path, pathlen, "%s/ralcalc_result", xdg_data);
		if(!path){
			fprintf(stderr, "Error: Out of memory.\n");
			return;
		}
		rc = readLastResultPath(value, path);
		free(path);
		if(rc == 0) return;
	}

	home = getenv("HOME");
	if(home){
		pathlen = strlen(home) + strlen("/.local/share/ralcalc_result") + 1;
		path = (char *)malloc(pathlen * sizeof(char));
		if(!path){
			fprintf(stderr, "Error: Out of memory.\n");
			return;
		}
		snprintf(path, pathlen, "%s/.local/share/ralcalc_result", home);

		rc = readLastResultPath(value, path);
		free(path);

		if(rc == 0) return;
	}

	{
		/* Loading from $HOME/.local/share/ralcalc_result failed, try old location. */
		pathlen = strlen(home) + strlen("/.ralcalc_result") + 1;
		path = (char *)malloc(pathlen * sizeof(char));
		if(!path){
			fprintf(stderr, "Error: Out of memory.\n");
			return;
		}
		snprintf(path, pathlen, "%s/.ralcalc_result", home);

		readLastResultPath(value, path);
		free(path);
	}

}


int main(int argc, char *argv[])
{
	int i;
	int rc = 0;
	char *ifile = NULL;
	FILE *iptr;
	struct ralcalc_config config;

	memset(&config, 0, sizeof(struct ralcalc_config));
	config.dm = dmSI;
	config.precision = -1;

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


	load_config(&config);

	for(i = 1; i < argc; i++){
		if(!strcmp(argv[i], "-q")){
			config.quiet = 1;
			argv[i][0] = '\0';
		}else if(!strcmp(argv[i], "-e")){
			config.dm = dmExponent;
			argv[i][0] = '\0';
		}else if(!strcmp(argv[i], "-f")){
			if(i < argc - 1){
				config.ifile = strdup(argv[i+1]);
				argv[i][0] = '\0';
				argv[i+1][0] = '\0';
				i++;
			}else{
				printUsage();
				return 1;
			}
		}else if(!strcmp(argv[i], "-i")){
			config.usestdin = 1;
		}else if(!strcmp(argv[i], "-p")){
			if(i < argc - 1){
				config.precision = atoi(argv[i+1]);
				argv[i][0] = '\0';
				argv[i+1][0] = '\0';
				i++;
				if(config.precision < 0){
					fprintf(stderr, _("Error: Precision must be a positive integer.\n"));
					return 1;
				}
			}else{
				printUsage();
				return 1;
			}
		}else if(!strcmp(argv[i], "-r")){
			config.dm = dmRaw;
			argv[i][0] = '\0';
		}else if(!strcmp(argv[i], "-s")){
			if(i < argc - 1){
				if(strlen(argv[i+1]) != 1){
					fprintf(stderr, _("Error: Invalid SI prefix '%s' for '-s' option.\n"), argv[i+1]);
					return 1;
				}else{
					config.dm = dmSI;
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
							config.siPrefix = argv[i+1][0];
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

	/* Do calculation based on input arguments first */
	if(doLineCalculation(argc, argv, &config)) rc = 1;

	/* Do calculations from a disk file */
	if(ifile){
		iptr = fopen(ifile, "rt");
		if(!iptr){
			fprintf(stderr, _("Error: Unable to open file \"%s\"\n"), ifile);
			return 1;
		}
		if(doFileInput(iptr, &config)) rc = 1;
		fclose(iptr);
	}

	/* Read calculations from stdin */
	if(config.usestdin){
		if(doFileInput(stdin, &config)) rc = 1;
	}

	return rc;
}

