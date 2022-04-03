#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "stack.h"
#include "mathexpression.h"
#include "errors.h"
#include "interpreter.h"


int main(int argc, char* argv[])
{
	printf(" ___________________________________________________\n");
	printf("|                                                   |\n");
	printf("|                  MATH INTERPRETER                 |\n");
	printf("|___________________________________________________|\n");
	printf("\nFor information about program write command: info\n");
	printf("If you want to exit write command: exit\n");
	char input[200] = {0};
	Variables vars;
	do_vars_empty(&vars);

	double result;
	while (1)
	{
		fgets(input, 1, stdin);
		printf(">>> ");
		fgets(input, 200, stdin);

		result = get_nan();
		Pair pairError = do_command_line(input, &vars, &result, argv[0]);
		if (pairError.number == EXIT_NUMBER) {
			return 0;
		}
		if (pairError.number) {
			print_error(pairError);
		}
		
		else {
			if (!isnan(result)) {
				printf("%lf\n", result);
			}
		}
		memset(input, '\0', sizeof(input));
	}
	return 0;
}
