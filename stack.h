#ifndef STACK_H
# define STACK_H

typedef union Elem {
    double num;
    char sym;
} Elem;

typedef struct Node {
    Elem value;
    struct Node * next;
} Node;


typedef struct Stack {
    Node *head;
} Stack;


Stack* create_stack();
int delete_stack(Stack **);
int push(Stack *, Elem);
int pop(Stack *, Elem *);
int top(Stack *, Elem *);


#endif // STACK_H