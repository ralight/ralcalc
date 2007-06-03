#ifndef DATATYPES_H
#define DATATYPES_H

typedef enum{
	tkNumber,
	tkMultiply = '*', // 42
	tkPlus = '+', // 43
	tkMinus = '-', // 45
	tkDivide = '/', // 47
	tkOpenBracket = '[', // 91
	tkCloseBracket = ']', // 93
	tkPower = '^', // 94
	tkExponent = 'e', // 101
	tkMultiplyX = 'x', // 120

	tkEndToken
} cToken;

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

typedef struct _tokenItem{
	struct _tokenItem *next;
	struct _tokenItem *prev;
	cToken type;
	double value;
	int length;
} tokenItem;

#endif
