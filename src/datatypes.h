/*
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
 */

#ifndef DATATYPES_H
#define DATATYPES_H

/* cToken
 *
 * The different types of token that are valid.  This does not mean the
 * different characters that are valid, although for the single character
 * tokens the identifier matches the ascii value for the character. tkNumber on
 * the other hand includes all numbers, the decimal point and the SI-prefix
 * characters as well.
 * tkNumber may also include "e" for numbers of the form 1e-3.
 */
typedef enum{
	tkNumber,
	tkMod = '%', // 37
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

	tkLog = 1000,
	tkLn,
//	tkExp,
	tkSin,
	tkCos,
	tkTan,
//	tkASin,
//	tkACos,
//	tkATan,

/* Constants */
	tkPi,

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
	errBadFile,
	errUnknownToken
} errType;


/* tokenItem
 *
 * Token struct containing all information about a particular token,
 * implemented as a doubly linked list.
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
