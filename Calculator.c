#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "stack.h"
extern int intTop;

const int MAX = 100;

int precedence(char operator) {
    switch (operator) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        default: return 0;
    }
}

bool isOperator(char symbol) {
    return (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/');
}

int infixToPostfix(char *infix, char postfix[][100]) {
    int i = 0, k = 0;
    while (infix[i] != '\0') {
        if (isspace(infix[i])) {
            i++;
            continue;
        }
        if (isdigit(infix[i])) {
            char number[100];
            int j = 0;
            while (isdigit(infix[i])) {
                number[j++] = infix[i++];
            }
            number[j] = '\0';
            strcpy(postfix[k++], number);
        } 
        else if (isOperator(infix[i])) {
            while (!isEmptyOperator() &&
                   precedence(peekOperator()) >= precedence(infix[i])) {
                char op[2] = { popOperator(), '\0' };
                strcpy(postfix[k++], op);
            }
            pushOperator(infix[i]);
            i++;
        } 
        else {
            return -1; 
        }
    }
    while (!isEmptyOperator()) {
        char op[2] = { popOperator(), '\0' };
        strcpy(postfix[k++], op);
    }
    return k;
}

int evalPostfix(char postfix[][100], int len, int *errorFlag) {
    intTop = -1;
    for (int i = 0; i < len; i++) {
        if (isdigit(postfix[i][0])) {
            int value = 0;
            for (int j = 0; postfix[i][j] != '\0'; j++) {
                value = value * 10 + (postfix[i][j] - '0');
            }
            pushInt(value);
        } 
        else {
            if (intTop < 1) {
                *errorFlag = 2; 
                return 0;
            }
            int b = popInt();
            int a = popInt();
            switch (postfix[i][0]) {
                case '+': pushInt(a + b); break;
                case '-': pushInt(a - b); break;
                case '*': pushInt(a * b); break;
                case '/':
                    if (b == 0) {
                        *errorFlag = 1;
                        return 0;
                    }
                    pushInt(a / b);
                    break;
                default:
                    *errorFlag = 2;
                    return 0;
            }
        }
    }
    return popInt();
}

int main() {
    char infix[100];
    char postfix[100][100];
    int length, errorFlag;

    while (true) {
        printf("\nEnter an infix expression (or type 'exit' to quit): ");
        if (!fgets(infix, MAX, stdin)) {
            printf("Error: Invalid input.\n");
            continue;
        }

        int len = strlen(infix);
        if (infix[len - 1] == '\n') infix[len - 1] = '\0';

        if (strcmp(infix, "exit") == 0) {
            printf("Exiting program...\n");
            break;
        }

        length = infixToPostfix(infix, postfix);
        if (length == -1) {
            printf("Error: Invalid expression.\n");
            continue;
        }

        errorFlag = 0;
        int result = evalPostfix(postfix, length, &errorFlag);

        switch (errorFlag) {
            case 1: 
                printf("Error: Division by zero.\n"); 
                break;

            case 2: 
                printf("Error: Invalid expression.\n"); 
                break;

            default: 
                printf("Result: %d\n", result); 
                break;
        }
    }

    return 0;
}
