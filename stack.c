#include "stdlib.h"
#include "stack.h"

Stack* create_stack()
{
    Stack *tmp = (Stack*) malloc(sizeof(Stack));
    tmp->head = NULL;
    return tmp;
}


int delete_stack(Stack **stack) {
    if (*stack == NULL)
        return 1;
    Node *tmp = (*stack)->head;
    Node *next = NULL;
    while (tmp)
    {
        next = tmp->next;
        free(tmp);
        tmp = next;
    }
    free(*stack);
    (*stack) = NULL;
    return 0;
}

int push(Stack *stack, Elem data) {
	Node *tmp = (Node*) malloc(sizeof(Node));
	if (stack == NULL)
	    return 1;
	tmp->value = data;
	tmp->next = stack->head;
	stack->head = tmp;
	return 0;
}



int pop(Stack *stack, Elem *to_save_here) {
	Node *prev;
	if (stack == NULL)
		return 1;
	
	if (stack->head == NULL)
		return 2;

	prev = stack->head;
	stack->head = stack->head->next;
	
	*to_save_here = prev->value;
	free(prev);

	return 0;
}


int top(Stack *stack, Elem * to_save_here)
{
    if (stack == NULL)
		return 1;
	if (stack->head == NULL)
		return 2;
    *to_save_here = stack->head->value;
    return 0;
}



