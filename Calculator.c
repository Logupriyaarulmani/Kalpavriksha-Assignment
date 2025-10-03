#include<stdio.h>
#include<string.h>
#include<ctype.h>
#define MAX 100
int intStack[MAX];
int intTop=-1;
void pushInt(int val){
    intStack[++intTop]=val;
}
int popInt(){
    return intStack[intTop--];
}
int isEmptyInt(){
    return intTop==-1;
}
char charStack[MAX];
int charTop = -1;
void pushChar(char c){
    charStack[++charTop]=c;
}
char popChar(){
    return charStack[charTop--];
}
char peekChar(){
    return charStack[charTop];
}
int isEmptyChar(){
    return charTop==-1;
}
int precedence(char op){
    if (op=='+'||op=='-') return 1;
    if (op=='*'||op=='/') return 2;
    return 0;
}
int isOperator(char c){
    return (c=='+'|| c=='-'||c=='*'||c=='/');
}
int infixToPostfix(char *infix,char postfix[][MAX]){
    int i=0,k=0;
    while(infix[i]!='\0'){
        if(isspace(infix[i])){ 
            i++; 
            continue; 
        }
        if(isdigit(infix[i])){
            char num[MAX];
            int j=0;
            while(isdigit(infix[i])){
                num[j++] = infix[i++];
            }
            num[j]='\0';
            strcpy(postfix[k++],num);
        }
        else if(isOperator(infix[i])){
            while(!isEmptyChar()&&precedence(peekChar())>=precedence(infix[i])){
                char op[2];
                op[0]=popChar();
                op[1]='\0';
                strcpy(postfix[k++],op);
            }
            pushChar(infix[i]);
            i++;
        }
        else{
            return -1;
        }
    }
    while(!isEmptyChar()){
        char op[2];
        op[0]=popChar();
        op[1]='\0';
        strcpy(postfix[k++],op);
    }
    return k;
}
int finalResult;
int evalPostfix(char postfix[][MAX], int len){
    intTop = -1;
    for (int i=0;i<len;i++){
        if (isdigit(postfix[i][0])){
            int val=0;
            for (int j=0;postfix[i][j]!='\0';j++){
                val=val*10+(postfix[i][j]-'0');
            }
            pushInt(val);
        }
        else {
            if (intTop < 1) return 2;
            int b=popInt();
            int a=popInt();
            if (postfix[i][0]=='+') pushInt(a+b);
            else if (postfix[i][0]=='-') pushInt(a-b);
            else if (postfix[i][0]=='*') pushInt(a*b);
            else if (postfix[i][0]=='/'){
                if (b==0) return 1;
                pushInt(a/b);
            }
        }
    }
    finalResult=popInt();
    return 0;
}
int main(){
    char infix[MAX];
    if (!fgets(infix, MAX, stdin)){
        printf("Error:Invalid expression.\n");
        return 0;
    }
    int len=strlen(infix);
    if (infix[len-1] == '\n') infix[len-1]='\0';
    char postfix[MAX][MAX];
    len=infixToPostfix(infix,postfix);
    if (len==-1){
        printf("Error:Invalid expression.\n");
        return 0;
    }
    int error=evalPostfix(postfix,len);
    if (error==1) printf("Error:Division by zero.\n");
    else if (error==2) printf("Error:Invalid expression.\n");
    else printf("%d\n",finalResult);
    return 0;
}
