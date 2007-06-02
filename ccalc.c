#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
	tkNumber,
	tkPlus,
	tkMinus,
	tkMultiply,
	tkDivide,
	tkPower,
	tkOpenBracket,
	tkCloseBracket,
	/* these below should be parsed as a number
	tkExponent,
	tkZetta,
	tkExa,
	tkPeta,
	tkTera,
	tkGiga,
	tkMega,
	tkKilo,
	tkMilli,
	tkMicro,
	tkNano,
	tkPico,
	tkFemto,
	tkAtto,
	tkZepto,
	tkYocto,
	*/

	tkEndToken
} cToken;

int addToken(void *tokenList, cToken token)
{
	switch(token){
		case tkPlus:
			printf("+\n");
			break;
		case tkMinus:
			printf("-\n");
			break;
		case tkMultiply:
			printf("x\n");
			break;
		case tkDivide:
			printf("/\n");
			break;
		case tkPower:
			printf("^\n");
			break;
		case tkOpenBracket:
			printf("[\n");
			break;
		case tkCloseBracket:
			printf("]\n");
			break;
		default:
			printf("Unknown token\n");
			return 1;
			break;
	}

	return 0;
}

int addNumber(void *tokenList, const char *buffer, int bufferPos)
{
	int i;
	char str[bufferPos+2];
	int haveMultiplier = 0;
	int haveDecimalPoint = 0;
	int multiplierPos = -1;
	double multiplier = 1.0;
	double number = 0.0;

	strncpy(str, buffer, bufferPos+2);

	for(i = bufferPos-1; i >= 0; i--){
		switch(str[i]){
			case 'Y':
				multiplier = 1000000000000000000000000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'Z':
				multiplier = 1000000000000000000000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'E':
				multiplier = 1000000000000000000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'P':
				multiplier = 1000000000000000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'T':
				multiplier = 1000000000000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'G':
				multiplier = 1000000000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'M':
				multiplier = 1000000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'k':
				multiplier = 1000.0;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'm':
				multiplier = 0.001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'u':
				multiplier = 0.000001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'n':
				multiplier = 0.000000001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'p':
				multiplier = 0.000000000001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'f':
				multiplier = 0.000000000000001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'a':
				multiplier = 0.000000000000000001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'z':
				multiplier = 0.000000000000000000001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case 'y':
				multiplier = 0.000000000000000000000001;
				haveMultiplier++;
				multiplierPos = i;
				str[i] = ' ';
				break;
			case '.':
				haveDecimalPoint++;
		}
	}

	if(haveMultiplier > 1 || haveDecimalPoint > 1) return 1;
	if(haveMultiplier && multiplierPos != bufferPos - 1) return 1;

	number = strtod(str, NULL);
	printf("buffer = %s, str = %s, number = %g, scaled = %g\n", buffer, str, number, number*multiplier);

	return 0;
}

int tokenise(const char *line)
{
	int i;
	void *tokenList;
	cToken lastToken = tkEndToken;
	char buffer[100];
	int bufferPos = 0;
	int inNumber = 0;

	if(!line) return -2;

	memset(buffer, 0, 100);

	for(i = 0; i < strlen(line); i++){
		printf("%d %s.\n", i, buffer);
		switch(line[i]){
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '.':
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
				if(inNumber){
					buffer[bufferPos] = line[i];
					bufferPos++;
				}else{
					memset(buffer, 0, 100);
					buffer[0] = line[i];
					bufferPos = 1;
					inNumber = 1;
				}
				break;

			case '+':
				if(inNumber){
					if(addNumber(tokenList, buffer, bufferPos)) return i;
					inNumber = 0;
				}
				if(addToken(tokenList, tkPlus)) return i;
				lastToken = tkPlus;
				break;
			case '-':
				if(inNumber){
					if(addNumber(tokenList, buffer, bufferPos)) return i;
					inNumber = 0;
				}
				if(addToken(tokenList, tkMinus)) return i;
				lastToken = tkPlus;
				break;
			case 'x':
				if(inNumber){
					if(addNumber(tokenList, buffer, bufferPos)) return i;
					inNumber = 0;
				}
				if(addToken(tokenList, tkMultiply)) return i;
				lastToken = tkPlus;
				break;
			case '/':
				if(inNumber){
					if(addNumber(tokenList, buffer, bufferPos)) return i;
					inNumber = 0;
				}
				if(addToken(tokenList, tkDivide)) return i;
				lastToken = tkPlus;
				break;
			case '[':
				if(inNumber){
					if(addNumber(tokenList, buffer, bufferPos)) return i;
					inNumber = 0;
				}
				if(addToken(tokenList, tkOpenBracket)) return i;
				lastToken = tkPlus;
				break;
			case ']':
				if(inNumber){
					if(addNumber(tokenList, buffer, bufferPos)) return i;
					inNumber = 0;
				}
				if(addToken(tokenList, tkCloseBracket)) return i;
				lastToken = tkPlus;
				break;
		}
	}
	if(inNumber){
		if(addNumber(tokenList, buffer, bufferPos)) return i;
		inNumber = 0;
	}

	return -1;
}

int main(int argc, char *argv[])
{
	int i, j, k;
	char *line = NULL;
	int len = 1;
	int rc;

	for(i = 1; i < argc; i++){
		len += strlen(argv[i]);
	}

	line = calloc(len, sizeof(char));

	i = 0;
	for(j = 1; j < argc; j++){
		for(k = 0; k < strlen(argv[j]); k++){
			if(argv[j][k] != ' '){
				line[i] = argv[j][k];
				i++;
			}
		}
	}

	printf("%s\n", line);

	rc = tokenise(line);
	if(rc>=0){
		printf("Error in input:\n%s\n", line);
		for(i = 0; i < rc - 1; i++){
			printf(" ");
		}
		printf("^\n");
	}
	free(line);

	return 0;
}

