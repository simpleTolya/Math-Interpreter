#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "mathexpression.h"
#include "errors.h"
#include "interpreter.h"
#include "errors.h"

void printf_info()
{
    printf("Coursework of Nyagin Anatoliy, group M8O-110B-20\n");
    printf("This mathematical interpreter can calculate the values of mathematical expressions.\n");
    printf("To do this, write a mathematical expression in the console after >>>\n");
    printf("Operations +, -, *, /, ^ are supported.\n");
    printf("For example, >>> -1+1\n\n");
    
    printf("It is also possible to define single-letter variables.\n");
    printf("To do this, write the name of the variable, sign =, the value of the variable (mathematical expression).\n");
    printf("For example, >>> a = (b + 2)*-2\n\n");
    
    printf("You can find out the value of a particular variable by writing its name.\n");
    printf("For example, >>> a\n\n");
    
    printf("You can save the values of variables defined during the program using the command: save <file_name>\n");
    printf("Then the values ​​of the variables will be saved in a file <file_name> in the directory containing the program.\n");
    printf("For example, >>> save VarValues.txt\n\n");
    
    printf("You can execute pre-recorded commands from a file with the command: run <file_name>\n");
    printf("File <file_name> must be stored in the same directory where the program is stored.\n");
    printf("For example, >>> run linesforprog.txt\n\n");
    
    printf("To turn off the program, write the command: exit\n");
    printf("For example, >>> exit\n");
}

void save_vars_to_file(const char file_name[], const Variables *vars)
{
    FILE * file = fopen(file_name, "w");
    int i=0;
    for (i=0; i < 26; i++) {
        if (!isnan(vars->val[i])) {
            fprintf(file, "%c = %lf\n", i+'a', vars->val[i]);
        }
    }
    
    for (i=26; i < 52; i++) {
        if (!isnan(vars->val[i])) {
            fprintf(file, "%c = %lf\n", i+'A'-26, vars->val[i]);
        }
    }
    fclose(file);
}


Pair run_commands_from_file(const char file_name[], Variables *vars)
{
    FILE * file = fopen(file_name, "r");
    Pair pairError;
    pairError.number = 0;
    if (file == NULL) {
        pairError.number = FILE_NOT_FOUND_ERROR;
        return pairError;
    }
    int line_index=1;
    char file_line[200]={0};
    double result;
    for (line_index=1; !feof(file); line_index++) {
        result = get_nan();
        fgets(file_line, 200, file);
        pairError = do_command_line(file_line, vars, &result, file_name);
        if (pairError.number == EXIT_NUMBER) {
            fclose(file);
            return pairError;
        }
        if (pairError.number) {
            fprintf(stderr, "In <%s> file in line number %d\n",
            file_name, line_index);
            fclose(file);
            return pairError;
        }
        if (!isnan(result)) {
            printf("Line %d - %lf\n", line_index, result); 
        }
        
    }
    fclose(file);
    return pairError;
}



Pair do_command_line(const char line[], Variables * vars, double *result,
    const char program_name[]) {

    int i=0;
    for (i = 0; line[i] == ' ';) {
        i++;
    }
    
    Pair pairError;
    pairError.number = 0;
    pairError.index = i+1;
    
    if (line[i+1] == '\0') {
        return pairError;
    }

    if (!(maybe_is_correct_sym(line[i]) || (line[i] == '='))) {
        pairError.number = CHAR_IS_NOT_ALLOWED_ERROR;
        return pairError;
    }
    if (isalpha(line[i])) {
        
        if (isalpha(line[i+1])) {
        
            char file_name[30]={0};
            int j = 0;
            if (strncmp(line+i, "exit", 4) == 0) {
                for (i=i+4; line[i+1];i++) {
                    if (line[i] != ' ') {
                        pairError.index = i+1;
                        pairError.number = 
                        AFTER_COMM_NOT_EXPECTED_CHARS_ERROR;
                        return pairError;
                    }
                }
                save_vars_to_file("ProgramVarsAfterExit.txt", vars);
                pairError.number = EXIT_NUMBER;
                return pairError;
            }
            
            if (strncmp(line+i, "info", 4) == 0) {
                for (i=i+4; line[i+1];i++) {
                    if (line[i] != ' ') {
                        pairError.index = i+1;
                        pairError.number = 
                        AFTER_COMM_NOT_EXPECTED_CHARS_ERROR;
                        return pairError;
                    }
                }
                printf_info();
                pairError.number = 0;
                return pairError;
            }
            
            if (strncmp(line+i, "save", 4) == 0) {
                for (i=i+4; line[i] == ' ';) {
                    i++;
                }
                for (; line[i+1]; i++) {
                    file_name[j] = line[i];
                    j++;
                }
                if (file_name[0] == '\0') {
                    pairError.number = FILE_NAME_NOT_MET_ERROR;
                    return pairError;
                }
                if (!strcmp(file_name, program_name)) {
                    pairError.number = FILE_IS_RUNNING_ERROR;
                    return pairError;
                }
                save_vars_to_file(file_name, vars);
                pairError.number = 0;
                return pairError;
            }
            
            if (strncmp(line+i, "run", 3) == 0) {
                for (i=i+3; line[i] == ' ';) {
                    i++;
                }
                for (; line[i+1]; i++) {
                    file_name[j] = line[i];
                    j++;
                }
                if (file_name[0] == '\0') {
                    pairError.number = FILE_NAME_NOT_MET_ERROR;
                    return pairError;
                }
                if (!strcmp(file_name, program_name)) {
                    pairError.number = FILE_IS_RUNNING_ERROR;
                    return pairError;
                }
                pairError = run_commands_from_file(file_name, vars);
                pairError.index = i+1;
                return pairError;
            }
            // else
            pairError.number = COMMAND_NOT_EXIST_ERROR;
            return pairError;
        }
        
        else {
            if (strstr(line, "=")) {
                // A = B
                char var = line[i];
                for (i = i+1; line[i] == ' ';) {
                    i++;
                }
                if (line[i] == '=') {
                    double res;
                    pairError = expression_value(line+i+1, vars, &res);
                    if (pairError.number == 0) {
                        vars->val[var_index(var)] = res;
                        return pairError;
                    }
                    else {
                        pairError.index += i;
                        return pairError;
                    }
                }
                else {
                    pairError.number = MUST_BE_NO_CHARS_BETWEEN_ERROR;
                    pairError.index = i;
                    return pairError;
                }
            }
            else { //math expression
                pairError = expression_value(line, vars, result);
                if (pairError.number) {
                    return pairError;
                }
            }
        }
    }
    if (line[i] == '=') {
        pairError.number = LEFT_PART_NOT_MET_ERROR;
        return pairError;
    }
    if (isdigit(line[i]) || is_oper(line[i]) || (line[i] == '(') ||
     (line[i] == ')') || (line[i] == '.')) {
        pairError = expression_value(line, vars, result);
        if (pairError.number) {
            return pairError;
        }
    }
    return pairError;
    
}


