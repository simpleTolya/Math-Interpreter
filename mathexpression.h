#ifndef MATH_EXPRESSION_H
# define MATH_EXPRESSION_H

typedef struct Variables {
    double val[52];
} Variables;

typedef struct Pair {
    int number;
    int index;
} Pair;


double get_nan();
void do_vars_empty(Variables *);
int is_only_binary_oper(char );
int is_oper(char );
int maybe_is_correct_sym(char );
double operate(double , char , double );
int priority(char );
int var_index(char );
double var_value(char , const Variables *);
Pair expression_value(const char[], const Variables *, double *);


#endif // MATH_EXPRESSION_H