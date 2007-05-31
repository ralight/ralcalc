#include <stdio.h>
#include <stdlib.h>

/*
token_t *tokenise(int argc, char *argv[])
{
}
*/

int main(int argc, char *argv[])
{
	double one, two;
	char operator;
	int i, j, k;
	char *line = NULL;
	int len = 1;

	for(i = 1; i < argc; i++){
		len += strlen(argv[i]);
	}

	line = calloc(len, sizeof(char));

	i = 0;
	for(j = 1; j < argc; j++){
		for(k = 0; k < strlen(argv[j]); k++){
			line[i] = argv[j][k];
			i++;
		}
	}

	printf("%s\n", line);

	one = atof(argv[1]);
	operator = argv[2][0];
	two = atof(argv[3]);

	switch(operator){
		case '+':
			printf("%g %c %g = %g\n", one, operator, two, one + two);
			break;
		case '-':
			printf("%g %c %g = %g\n", one, operator, two, one - two);
			break;
		case 'x':
			printf("%g %c %g = %g\n", one, operator, two, one * two);
			break;
		case '/':
			if(two != 0.0){
				printf("%g %c %g = %g\n", one, operator, two, one / two);
			}else{
				printf("%g %c %g = NaN\n", one, operator, two);
			}
			break;
		default:
			printf("Unknown operator '%c'\n", operator);
			free(line);
			return 1;
	}

	free(line);

	return 0;
}

