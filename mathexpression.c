#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "stack.h"
#include "mathexpression.h"
#include "errors.h"

double get_nan()
{
	return -pow(-1, 0.5);
}

void do_vars_empty(Variables *vars)
{
	for (int i=0; i<52; i++)
		vars->val[i] = get_nan();
}

int is_only_binary_oper(char ch)
{
	if ((ch == '+') || (ch == '*') || (ch == '/') || (ch == '^'))
		return 1;
	return 0;
}

int is_oper(char ch)
{
	if ((ch == '+') || (ch == '-') || (ch == '*') ||
		(ch == '/') || (ch == '^'))
		return 1;
	return 0;
}

int maybe_is_correct_sym(char ch)
{
	if ((isalnum(ch)) || (is_oper(ch)) || (ch == ' ') || 
	(ch == '(') || (ch == ')') || (ch == '.')) {
		return 1;
	}
	return 0;
}


double operate(double arg1, char operation, double arg2)
{
	if (operation == '~')
		return -arg1;
	if (operation == '+')
		return arg1 + arg2;
	if (operation == '-')
		return arg1 - arg2;
	if (operation == '*')
		return arg1 * arg2;
	if (operation == '/')
		return arg1 / arg2;
	if (operation == '^')
		return pow(arg1, arg2);
	return get_nan();
}

int priority(char operation)
{
	if ((operation == '-') || (operation == '+'))
		return 1;
	if ((operation == '*') || (operation == '/'))
		return 2;
	if ((operation == '^') || (operation == '~'))
		return 3;
	return 0;
}

int var_index(char var)
{
	if (islower(var))
		return (var - 'a');
	if (isupper(var))
		return (var - 'A' + 26);
	return -1;
}

double var_value(char var, const Variables * vars)
{
	if (var_index(var) != -1)
		return vars->val[var_index(var)];
	return get_nan();
}

Pair expression_value(const char expression[], const Variables *vars,
    double *result) {
	Pair pairError;
	pairError.number = 0;
	pairError.index = 0;

	int i=0;
	for (i=0; expression[i] == ' ';) {
		i++;
	}
	pairError.index = i+1;

	if (expression[i+1] == '\0') {
		pairError.number = EMPTY_STRING_ERROR;
		return pairError;
	}
	int opened_closed_diff = 0;

	char str_number[20] = {0}; 
	char *after_last_digit = str_number; 
	Elem element; 
	int point_number = 0; 

	Elem top_elem;
	Elem num2;
	Elem num1;
	Elem oper;
	Elem res;

	Stack * numbers = create_stack();
	Stack * symbols = create_stack();

	if (!maybe_is_correct_sym(expression[i])) {
		pairError.number = CHAR_IS_NOT_ALLOWED_ERROR;
		delete_stack(&numbers);
		delete_stack(&symbols);
		return pairError;
	}
	if (expression[i] == ')') {
		pairError.number = OPENING_BRACKET_NOT_MET_ERROR;
		delete_stack(&numbers);
		delete_stack(&symbols);
		return pairError;
	}
	if (expression[i] == '.') {
		pairError.number = BEFORE_POINT_EXPECTED_DIGIT_ERROR;
		delete_stack(&numbers);
		delete_stack(&symbols);
		return pairError;
	}
	if (is_only_binary_oper(expression[i])) {
		pairError.number = BEFORE_OPERATION_EXPECTED_ERROR;
		delete_stack(&numbers);
		delete_stack(&symbols);
		return pairError;
	}
	if (expression[i] == '(') {
		opened_closed_diff++;
		
		element.sym = expression[i];
		push(symbols, element);
	}

	if (expression[i] == '-') {
		element.sym = '~';
		push(symbols, element);
	}

	if (isalpha(expression[i])) {
		element.num = var_value(expression[i], vars);
		if (isnan(element.num)) {
			pairError.number = VAR_NOT_DEFINED_ERROR;
			delete_stack(&numbers);
			delete_stack(&symbols);
			return pairError;
		}
		push(numbers, element);
	}

	if (isdigit(expression[i])) {
		*after_last_digit = expression[i];
		after_last_digit++;
		if (!(isdigit(expression[i+1])) && expression[i+1] != '.') {
			element.num = atof(str_number);
			memset(str_number, 0, sizeof(str_number));
			push(numbers, element);
			after_last_digit = str_number;
		}
	}



	char prev = expression[i];
	for (i=i+1; expression[i+1]; i++) {

		pairError.index = i+1;
		if (!maybe_is_correct_sym(expression[i])) {
			pairError.number = CHAR_IS_NOT_ALLOWED_ERROR;
			delete_stack(&numbers);
			delete_stack(&symbols);
			return pairError;
		}
		if (expression[i] == ' ')
			continue;
		
		if (isdigit(expression[i])) {
			if (isdigit(prev) && expression[i-1] == ' ') {
				pairError.number = AFTER_OPERAND_EXPECTED_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			if (isalpha(prev)) {
				pairError.number = AFTER_OPERAND_EXPECTED_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			if (expression[i] == ')') {
				pairError.number = AFTER_OPERAND_EXPECTED_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			*after_last_digit = expression[i];
			after_last_digit++;
			if (!(isdigit(expression[i+1])) && expression[i+1] != '.') {
				element.num = atof(str_number);
				memset(str_number, 0, sizeof(str_number));
				push(numbers, element);
				after_last_digit = str_number;
			}
		}
		
		if (isalpha(expression[i])) {
			if (prev == '.') {
				pairError.number = AFTER_POINT_EXPECTED_DIGIT_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			if (isdigit(prev)) {
				pairError.number = AFTER_OPERAND_EXPECTED_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			if (prev == ')') {
				pairError.number = AFTER_OPERAND_EXPECTED_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			if (isalpha(prev)) {
				pairError.number = AFTER_OPERAND_EXPECTED_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			point_number = 0;
			
			element.num = var_value(expression[i], vars);
			if (isnan(element.num)) {
				pairError.number = VAR_NOT_DEFINED_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			push(numbers, element);
		}
		
		if (is_only_binary_oper(expression[i])) {
			if (prev == '.') {
				pairError.number = AFTER_POINT_EXPECTED_DIGIT_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			if (prev == '(') {
				pairError.number = BEFORE_OPERATION_EXPECTED_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			if (is_oper(prev)) {
				pairError.number = BEFORE_OPERATION_EXPECTED_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			point_number = 0;
			
			if (symbols->head == NULL) {
				element.sym = expression[i];
				push(symbols, element);
				prev = expression[i];
				continue;
			}
			
			top(symbols, &top_elem);
			if (!(is_oper(top_elem.sym) || top_elem.sym == '~')) {
				element.sym = expression[i];
				push(symbols, element);
				prev = expression[i];
				continue;
			}
			
			if (expression[i] == '^') {
				if (top_elem.sym == '^' || top_elem.sym == '~') {
					element.sym = expression[i];
					push(symbols, element);
					prev = expression[i];
					continue;
				}
			}
			
			if (priority(expression[i]) > priority(top_elem.sym)) {
				element.sym = expression[i];
				push(symbols, element);
				prev = expression[i];
				continue;
			}
			else {
				top(symbols, &top_elem);
				while (((symbols->head != NULL) && (is_oper(top_elem.sym)|| top_elem.sym == '~')) && 
				(priority(expression[i]) <= priority(top_elem.sym))) {
					if (top_elem.sym == '~') {
						pop(numbers, &num1);
						pop(symbols, &oper);
						res.num = operate(num1.num, oper.sym, 0);
						push(numbers, res);
						top(symbols, &top_elem);
						continue;
					}
					pop(numbers, &num2);
					pop(numbers, &num1);
					pop(symbols, &oper);
					if ((oper.sym == '/') && (num2.num == 0)) {
						pairError.number = ZERO_DIVISION_ERROR;
						delete_stack(&numbers);
						delete_stack(&symbols);
						return pairError;
					}
					if ((oper.sym == '^') && (num1.num <= 0) &&
					(num2.num != trunc(num2.num))) {
						pairError.number = POWER_ERROR;
						delete_stack(&numbers);
						delete_stack(&symbols);
						return pairError;
					}
					res.num = operate(num1.num, oper.sym, num2.num);
					push(numbers, res);
					top(symbols, &top_elem);
				}
				element.sym = expression[i];
				push(symbols, element);
			}
		}
		
		if (expression[i] == '-') {
			if (prev == '.') {
				pairError.number = AFTER_POINT_EXPECTED_DIGIT_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			
			if (isalnum(prev) || prev == ')') {
				// binary minus
				if (symbols->head == NULL) {
					element.sym = '-';
					push(symbols, element);
					prev = expression[i];
					continue;
				}
				
				top(symbols, &top_elem);
				while ((symbols->head != NULL) && (is_oper(top_elem.sym)
				|| top_elem.sym == '~')) {
					if (top_elem.sym == '~') {
						pop(numbers, &num1);
						pop(symbols, &oper);
						res.num = operate(num1.num, oper.sym, 0);
						push(numbers, res);
						top(symbols, &top_elem);
						continue;
					}
					pop(numbers, &num2);
					pop(numbers, &num1);
					pop(symbols, &oper);
					if ((oper.sym == '/') && (num2.num == 0)) {
						pairError.number = ZERO_DIVISION_ERROR;
						delete_stack(&numbers);
						delete_stack(&symbols);
						return pairError;
					}
					if ((oper.sym == '^') && (num1.num <= 0) &&
					(num2.num != trunc(num2.num))) {
						pairError.number = POWER_ERROR;
						delete_stack(&numbers);
						delete_stack(&symbols);
						return pairError;
					}
					res.num = operate(num1.num, oper.sym, num2.num);
					push(numbers, res);
					top(symbols, &top_elem);
				}
				
				element.sym = '-';
				push(symbols, element);
			}
			if (prev == '(' || is_oper(prev)) {
				// unar minus
				if (symbols->head == NULL) {
					element.sym = '~';
					push(symbols, element);
					prev = expression[i];
					continue;
				}
				
				top(symbols, &top_elem);
				if (top_elem.sym == '^' || top_elem.sym == '~' ||
				!(is_oper(top_elem.sym))) {
					element.sym = '~';
					push(symbols, element);
					prev = expression[i];
					continue;
				}
				
				if (priority('~') > priority(top_elem.sym)) {
					element.sym = '~';
					push(symbols, element);
					prev = expression[i];
					continue;
				}
				else {
					top(symbols, &top_elem);
					while ((symbols->head != NULL) && (is_oper(top_elem.sym) || top_elem.sym == '~') &&
						(priority('~') <= priority(top_elem.sym))) {
						if (top_elem.sym == '~') {
							pop(numbers, &num1);
							pop(symbols, &oper);
							res.num = operate(num1.num, oper.sym, 0);
							push(numbers, res);
							top(symbols, &top_elem);
							continue;
						}
						pop(numbers, &num2);
						pop(numbers, &num1);
						pop(symbols, &oper);
						if ((oper.sym == '/') && (num2.num == 0)) {
							pairError.number = ZERO_DIVISION_ERROR;
							delete_stack(&numbers);
							delete_stack(&symbols);
							return pairError;
						}
						if ((oper.sym == '^') && (num1.num <= 0) &&
						(num2.num != trunc(num2.num))) {
							pairError.number = POWER_ERROR;
							delete_stack(&numbers);
							delete_stack(&symbols);
							return pairError;
						}
						res.num = operate(num1.num, oper.sym, num2.num);
						push(numbers, res);
						top(symbols, &top_elem);
					}
					element.sym = '~';
					push(symbols, element);
				}
			}
			
			point_number = 0;
		}
		
		if (expression[i] == '(') {
			if (prev == '.') {
				pairError.number = AFTER_POINT_EXPECTED_DIGIT_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			if (isdigit(prev)) {
				pairError.number = AFTER_OPERAND_EXPECTED_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			if (isalpha(prev)) {
				pairError.number = AFTER_OPERAND_EXPECTED_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			if (prev == ')') {
				pairError.number = AFTER_OPERAND_EXPECTED_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			opened_closed_diff++;
			point_number = 0;
			
			element.sym = expression[i];
			push(symbols, element);
		}
		
		if (expression[i] == ')') {
			if (prev == '(') {
				pairError.number = BRACKETS_IS_EMPTY_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			if (is_oper(prev)) {
				pairError.number = AFTER_OPERATION_EXPECTED_OPERAND_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			if (prev == '.') {
				pairError.number = AFTER_POINT_EXPECTED_DIGIT_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			opened_closed_diff--;
			if (opened_closed_diff < 0) {
				pairError.number = OPENING_BRACKET_NOT_MET_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			point_number = 0;
			
			top(symbols, &top_elem);
			while (top_elem.sym != '(') {
				if (top_elem.sym == '~') {
					pop(numbers, &num1);
					pop(symbols, &oper);
					res.num = operate(num1.num, oper.sym, 0);
					push(numbers, res);
					top(symbols, &top_elem);
					continue;
				}
				pop(numbers, &num2);
				pop(numbers, &num1);
				pop(symbols, &oper);
				if ((oper.sym == '/') && (num2.num == 0)) {
					pairError.number = ZERO_DIVISION_ERROR;
					delete_stack(&numbers);
					delete_stack(&symbols);
					return pairError;
				}
				if ((oper.sym == '^') && (num1.num <= 0) &&
				(num2.num != trunc(num2.num))) {
					pairError.number = POWER_ERROR;
					delete_stack(&numbers);
					delete_stack(&symbols);
					return pairError;
				}
				res.num = operate(num1.num, oper.sym, num2.num);
				push(numbers, res);
				top(symbols, &top_elem);
			}
			pop(symbols, &top_elem);
		}
		
		if (expression[i] == '.') {
			if (prev == '.') {
				pairError.number = POINT_IS_EXCESS_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			if (!(isdigit(prev))) {
				pairError.number = BEFORE_POINT_EXPECTED_DIGIT_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			point_number++;
			if (point_number > 1) {
				pairError.number = POINT_IS_EXCESS_ERROR;
				delete_stack(&numbers);
				delete_stack(&symbols);
				return pairError;
			}
			*after_last_digit = expression[i];
			after_last_digit++;
		}
		prev = expression[i];
	}

	if (opened_closed_diff) {
		pairError.number = NO_ALL_BRACKETS_CLOSED_ERROR;
		delete_stack(&numbers);
		delete_stack(&symbols);
		return pairError;
	}
	if (is_oper(expression[i-1])) {
		pairError.number = AFTER_OPERATION_EXPECTED_OPERAND_ERROR;
		delete_stack(&numbers);
		delete_stack(&symbols);
		return pairError;
	}
	if (expression[i-1] == '.') {
		pairError.number = AFTER_POINT_EXPECTED_DIGIT_ERROR;
		delete_stack(&numbers);
		delete_stack(&symbols);
		return pairError;
	}

	top(symbols, &top_elem);
	while (symbols->head != NULL) {
		if (top_elem.sym == '~') {
			pop(numbers, &num1);
			pop(symbols, &oper);
			res.num = operate(num1.num, oper.sym, 0);
			push(numbers, res);
			top(symbols, &top_elem);
			continue;
		}
		pop(numbers, &num2);
		pop(numbers, &num1);
		pop(symbols, &oper);
		if ((oper.sym == '/') && (num2.num == 0)) {
			pairError.number = ZERO_DIVISION_ERROR;
			delete_stack(&numbers);
			delete_stack(&symbols);
			return pairError;
		}
		if ((oper.sym == '^') && (num1.num <= 0) &&
		(num2.num != trunc(num2.num))) {
			pairError.number = POWER_ERROR;
			delete_stack(&numbers);
			delete_stack(&symbols);
			return pairError;
		}
		res.num = operate(num1.num, oper.sym, num2.num);
		push(numbers, res);
		top(symbols, &top_elem);
	}

	pop(numbers, &res);
	*result = res.num;
	delete_stack(&numbers);
	delete_stack(&symbols);
	return pairError;
}

