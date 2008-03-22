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

#ifndef DATATYPES_H
#define DATATYPES_H

/* cToken
 *
 * The different types of token that are valid.
 * This does not mean the different characters that
 * are valid, although for the single character tokens
 * the identifier matches the ascii value for the 
 * character. tkNumber on the other hand includes
 * all numbers, the decimal point and the SI-prefix
 * characters as well.
 * tkNumber *may* also include "e" in the future for
 * numbers of the form 1e-3.
 */
typedef enum{
	tkNumber,
	tkCOpenBracket = '(', // 40
	tkCCloseBracket = ')', // 41
	tkMultiply = '*', // 42
	tkPlus = '+', // 43
	tkMinus = '-', // 45
	tkDivide = '/', // 47
	tkOpenBracket = '[', // 91
	tkCloseBracket = ']', // 93
	tkPower = '^', // 94
	tkLastResult = '_', // 95 (result of last ralcalc run - functionally equivalent to tkNumber)
	tkMultiplyX = 'x', // 120
	tkNegation = '~', // 126

	tkEndToken
} cToken;


/* errType
 *
 * Possible errors for printing.
 */
typedef enum{
	errNoError,
	errBadInput,
	errNotFound,
	errMemory,
	errBadNumber,
	errDuplicateNumber,
	errMismatchedBrackets,
	errInvalidBracket,
	errInvalidOperator,
	errUnknownToken
} errType;


/* tokenItem
 *
 * Token struct containing all information about a particular
 * token, implemented as a doubly linked list.
 */
typedef struct _tokenItem{
	struct _tokenItem *next;
	struct _tokenItem *prev;
	cToken type;
	double value;
	int length;
	int precedence;
} tokenItem;

#endif
