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
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "calculation.h"
#include "datatypes.h"
#include "tokens.h"
#include "output.h"

void print_usage()
{
	printf("ralcalc  version %s\n", VERSION);
	printf("Copyright (C) 2007 Roger Light\nhttp://atchoo.org/tools/ralcalc/\n\n");
	printf("ralcalc comes with ABSOLUTELY NO WARRANTY.  You may distribute ralcalc freely\nas described in the LICENCE.txt distributed with this file.\n\n");
	printf("ralcalc is a simple command line calculator. \n\n");
	printf("Usage: ralcalc -h   (display this help)\n");
	printf("       ralcalc <an equation>\n");
	printf("\nSee http://atchoo.org/tools/ralcalc/ for updates.\n");
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
	
	if(argc==2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-v"))){
		print_usage();
		return 0;
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

	rc = tokenise(&tokenList, line, lastResult);
	if(rc>0) hasError = 1;

	rc = validate(&tokenList, line);
	if(rc>0){
		hasError = 1;
		thisItem = tokenList.next;
		errorPos = 0;

		while(thisItem && rc > 1){
			errorPos += thisItem->length;
			thisItem = thisItem->next;
			rc--;
		}

	}

	if(!hasError && tokenList.next){
		result = process(&(tokenList.next), line);
		doubleToString(result, resultStr, 100);
		printf("%s = %s\n", line, resultStr);

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

	return 0;
}

