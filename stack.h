
#include <stdbool.h>

static int intStack[100];
static int intTop = -1;

static void pushInt(int value) { intStack[++intTop] = value; }
static int popInt() { return intStack[intTop--]; }
static bool isEmptyInt() { return intTop == -1; }

static char operatorStack[100];
static int operatorTop = -1;

static void pushOperator(char op) { operatorStack[++operatorTop] = op; }
static char popOperator() { return operatorStack[operatorTop--]; }
static char peekOperator() { return operatorStack[operatorTop]; }
static bool isEmptyOperator() { return operatorTop == -1; }
