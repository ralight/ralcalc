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
	errBadNumber
} errType;

typedef struct _tokenItem{
	struct _tokenItem *next;
	cToken type;
	double value;
	int length;
} tokenItem;

int validate(tokenItem *tokenList);
int tokenise(tokenItem *tokenList, const char *line, char *errors);

#endif
