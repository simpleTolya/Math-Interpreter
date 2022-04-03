PROG_NAME = interpreter

all: $(PROG_NAME)

SRCS = errors.c interpreter.c main.c mathexpression.c stack.c
OBJS = $(SRCS:.c=.o)
FLAGS = -Wall -Wextra -Werror
CC = gcc

$(PROG_NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(PROG_NAME) 

%.o: %.c %.h
	$(CC) $(FLAGS) -c $< -o $@

main.o: main.c
	gcc -c main.c

clean:
	rm -rf *.o

fclean: clean
	rm -rf $(PROG_NAME)

re: fclean all

.PHONY = all clean fclean re