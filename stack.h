#include <stdbool.h>

void pushInt(int value);
int popInt(void);
bool isEmptyInt(void);

void pushOperator(char op);
char popOperator(void);
char peekOperator(void);
bool isEmptyOperator(void);