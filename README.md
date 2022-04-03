# Math-Interpreter

This application is able to calculate the values ​​of any mathematical
expressions with operations +, –, ×, ÷, ˆ. It is possible to define
single-letter variables (52 in total: a..z,A..Z), and then use the
defined variables in mathematical expressions. Certain variables
can be saved to a file using the appropriate save command. You can
run shell commands from another file with the appropriate run command.
You can get a brief help on the interpreter's capabilities and input
examples using the info command. The exit command terminates the interpreter,
clearing the memory allocated for the program, and writes certain variables to
a file named "ProgramVarsAfterExit.txt". The input data is always commands from
the user to the terminal. If you wish, you can run commands from a file, but
for this you need to write the run command with the name of the file to be
launched in the terminal in the running interpreter.

## Technologies

1. C
2. Make

## Launch

1. Build a project using Makefile

   ```sh
   $ make
   ```

2. Run the program with one argument (map), like this:

   ```sh
   $ ./interpreter
   ```
   
3. Commands:
  - info
  - exit
  - run [file_name]
  - save [file_name]

## Images
  <img src="./imgs/interpreter.gif" width="500" alt="game">