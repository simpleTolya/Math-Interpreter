#include <stdio.h>
#include <stdlib.h>
#include "mathexpression.h"
#include "errors.h"


void print_error(Pair pairError)
{
    FILE * file_stream = stderr;
    switch(pairError.number) {
        case CHAR_IS_NOT_ALLOWED_ERROR:
   	    {   
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "SyntaxError: the character is not allowed\n");
            break;
        }
        case BEFORE_OPERATION_EXPECTED_ERROR:
   	    {   
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "OperandNotFoundError: an operand expected before the operation\n");
            break;
        }
        case AFTER_OPERAND_EXPECTED_ERROR:
   	    {   
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "OperationNotFoundError: an operation expected before the operand\n");
            break;
        }
        case NO_ALL_BRACKETS_CLOSED_ERROR:
   	    {   
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "BracketError: not all brackets are closed\n");
            break;
        }
        case OPENING_BRACKET_NOT_MET_ERROR:
   	    {   
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "BracketError: an opening bracket was not encountered for this closing bracket\n");
            break;
        }
        case BEFORE_POINT_EXPECTED_DIGIT_ERROR:
   	    {   
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "PointError: a digit was expected before the dot\n");
            break;
        }
        case AFTER_POINT_EXPECTED_DIGIT_ERROR:
   	    {
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "PointError: a digit was expected after the dot\n");
            break;
        }
        case AFTER_OPERATION_EXPECTED_OPERAND_ERROR:
   	    {
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "OperandNotFoundError: an operand expected after the operation\n");
            break;
        }
        case BRACKETS_IS_EMPTY_ERROR:
   	    {   
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "SyntaxError: brackets are empty\n");
            break;
        }
        case POINT_IS_EXCESS_ERROR:
   	    {   
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "PointError: the point is excess\n");
            break;
        }
        case ZERO_DIVISION_ERROR:
   	    {   
            fprintf(file_stream, "ZeroDivisionError: division by zero\n");
            break;
        }
        case POWER_ERROR:
   	    {   
            fprintf(file_stream, "NotPositiePowerError: raising to a non-integer power of a not positive number\n");
            break;
        }
        case EMPTY_STRING_ERROR:
   	    {   
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "SyntaxError: math expression missing\n");
            break;
        }
        case VAR_NOT_DEFINED_ERROR:
   	    {   
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "VarNotDefinedError: the variable was not defined\n");
            break;
        }
        case LEFT_PART_NOT_MET_ERROR:
   	    {   
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "SyntaxError: missing left side of assignment\n");
            break;
        }
        case COMMAND_NOT_EXIST_ERROR:
   	    {   
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "CommandError: no such command exists\n");
            break;
        }
        case AFTER_COMM_NOT_EXPECTED_CHARS_ERROR:
   	    {   
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "SyntaxError: no characters expected after this command\n");
            break;
        }
        case FILE_NOT_FOUND_ERROR:
   	    {   
            fprintf(file_stream, "FileNotFoundError: no file to read in this directory was found\n");
            break;
        }
        case MUST_BE_NO_CHARS_BETWEEN_ERROR:
   	    {   
            fprintf(file_stream, "In the line at the character with number %d\n", pairError.index);
            fprintf(file_stream, "SyntaxError: there must be no characters between the variable and the = symbol\n");
            break;
        }
        case FILE_NAME_NOT_MET_ERROR:
   	    {   
            fprintf(file_stream, "SyntaxError: no file name encountered\n");
            break;
        }
        case FILE_IS_RUNNING_ERROR:
   	    {   
            fprintf(file_stream, "ImportError: trying to run the same file\n");
            break;
        }
    }
}



