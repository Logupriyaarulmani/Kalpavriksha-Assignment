char* removeOuterParentheses(char* s) {
    char* result = (char*) malloc (strlen(s) * sizeof(char));
    int depth = 0;
    int k = 0;

    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == '(') {
            if  (depth > 0) 
                result[k++] = '(';
            depth++;
        }
        else {
            depth --;
            if (depth > 0) 
                result[k++] = ')';
        }
    }
    result[k] = '\0';
    return result;
}