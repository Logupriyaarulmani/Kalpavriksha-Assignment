char* removeDuplicates(char* s) {
    char* result = (char*) malloc (strlen(s) * sizeof(s)+1);
    int top = -1;

    for (int i = 0; i != strlen(s); i++) {
        if (top != -1 && result[top] == s[i]) {
            top--;
        }
        else {
            result[++top] = s[i];
        }
    }
    result[top+1] = '\0';
    return result;
}