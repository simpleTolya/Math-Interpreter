#ifndef INTERPRETER_H
# define INTERPRETER_H

void printf_info();
void save_vars_to_file(const char[], const Variables *);
Pair do_command_line(const char[], Variables *, double *, const char []);
Pair run_commands_from_file(const char[], Variables *);

#endif