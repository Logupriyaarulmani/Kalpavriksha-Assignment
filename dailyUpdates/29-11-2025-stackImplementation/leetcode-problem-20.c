bool isValid(char* s) {
    int len = strlen(s);

    if (len % 2 != 0) {
        return false;
    }

    int top = -1;
    char* stack = (char*)malloc(len);

    for (int i = 0; i < len; i++){
        if (s[i] == '(' || s[i] == '[' || s[i] == '{') {
            stack[++top] = s[i];
        }
        else {
            if (top == -1) {
                free(stack);
                return false;
            }
            char popStack = stack[top--];
            if ((s[i] == ')' && popStack != '(') || (s[i] == ']' && popStack != '[') || 
                (s[i] == '}' && popStack != '{')) {
                    free(stack);
                    return false;
                }
        }
    }
    free(stack);
    return top == -1;
}