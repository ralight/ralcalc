#ifndef CCALC_H
#define CCALC_H

typedef enum{
	tkNumber,
	tkPlus = '+',
	tkMinus = '-',
	tkMultiply = 'x',
	tkDivide = '/',
	tkPower = '^',
	tkOpenBracket = '[',
	tkCloseBracket = ']',
	/* these below should be parsed as a number
	tkExponent,
	*/

	tkEndToken
} cToken;

typedef enum{
	errNoError,
	errBadInput,
	errMemory,
	errBadNumber,
	errMismatchedBrackets,
	errUnknownToken
} errType;

typedef struct _tokenItem{
	struct _tokenItem *next;
	cToken type;
	double value;
	int length;
} tokenItem;

void printError(const char *line, int pos, errType error);
int validate(tokenItem *tokenList, const char *line);
int tokenise(tokenItem *tokenList, const char *line);

#endif
