char* removeStars(char* s) {
    int len = strlen(s);

    char* stack = (char*)malloc(len + 1);
    int top = -1;
    for (int i = 0; i < len; i++) {
        if (s[i] != '*') {
            stack[++top] = s[i];
        }
        else if (top >=0) {
            top--;
        }
    }
    stack[top + 1] = '\0';
    return stack;
}