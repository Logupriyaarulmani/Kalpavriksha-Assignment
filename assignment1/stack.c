#include "stack.h"

static int intStack[100];
static int intTop = -1;

void pushInt(int value) { 
    intStack[++intTop] = value;
}
int popInt() { 
    return intStack[intTop--]; 
}
bool isEmptyInt() { 
    return intTop == -1;
}

static char operatorStack[100];
static int operatorTop = -1;

void pushOperator(char op) { 
    operatorStack[++operatorTop] = op; 
}
char popOperator() { 
    return operatorStack[operatorTop--]; 
}
char peekOperator() { 
    return operatorStack[operatorTop]; 
}
bool isEmptyOperator() { 
    return operatorTop == -1; 
}