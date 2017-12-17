#include<cstdio>
#include<cstring>
#include<iostream>
#include<list>

using namespace std;

#define TOKEN_STATE_WHITE   0
#define TOKEN_STATE_ID      1
#define TOKEN_STATE_INT     2
#define TOKEN_STATE_REAL    3

#define PARSE_STATE_START   10
#define PARSE_STATE_WAIT    11
#define PARSE_STATE_ID      12
#define PARSE_STATE_INT     13
#define PARSE_STATE_REAL    14
#define PARSE_STATE_PLUS    15
#define PARSE_STATE_MINUS   16
#define PARSE_STATE_MULTI   17
#define PARSE_STATE_DEVIDE  18
#define PARSE_STATE_OPEN_B  19
#define PARSE_STATE_CLOSE_B 20
#define PARSE_STATE_EQUAL   21
#define PARSE_STATE_SEMI_C  22
#define PARSE_STATE_EOL     23

#define ID      '0'
#define INT     '1'
#define REAL    '2'

#define S   'a'
#define Z   'b'
#define E   'c'
#define E_  'd'
#define T   'e'
#define T_  'f'
#define F   'g'
#define A   'h'

#define INPUT_LENGTH        201
#define CASE_TOTAL          69
#define CASE_LIMIT          69


int trueAns = 0;
int falseAns = 0;

char currentInput[INPUT_LENGTH];
list<char> inputToken;
list<char> parseStack;

int tokenState;
int parseState;

bool Tokenizer();
void PrintList();
void PrintStack();
bool Parser();
bool IsLiteral(char input);
bool IsLetter(char input);
bool IsDigit(char input);

main(){
    FILE *file = fopen("testcase_true.txt", "r");
    //FILE *file = fopen("testcase_false.txt", "r");
    for(int i = 0 ; i < CASE_LIMIT ; i++){
        inputToken.clear();
        parseStack.clear();
        tokenState = TOKEN_STATE_WHITE;
        parseState = PARSE_STATE_START;

        fgets(currentInput, INPUT_LENGTH, file);
        if(i != CASE_TOTAL - 1) currentInput[strlen(currentInput) - 1] = '\0'; //delete 'return' char
        printf("%s\n", currentInput);

        if(Tokenizer()){
            //PrintList();
        }
        else{
            printf("Tokenizing Failed\n");
            falseAns++;
            PrintList();
            continue;
        }

        if(Parser()){
            printf("Parsing Succeeded\n");
            trueAns++;
        }
        else{
            printf("Parsing Failed\n");
            PrintStack();
            falseAns++;
        }
        printf("\n");
    }
    printf("Correct : %d\nIncorrect : %d\n", trueAns, falseAns);
}

bool Tokenizer(){
    int inputLen = strlen(currentInput);
    for(int i = 0 ; i < inputLen ; i++){
        //printf("%c : %d\n", currentInput[i], tokenState);
        if(tokenState == TOKEN_STATE_WHITE){
            if(IsLiteral(currentInput[i])){
                inputToken.push_back(currentInput[i]);
            }
            else if(currentInput[i] == ' '){
                //do nothing
            }
            else if(IsLetter(currentInput[i])){
                tokenState = TOKEN_STATE_ID;
            }
            else if(IsDigit(currentInput[i])){
                tokenState = TOKEN_STATE_INT;
            }
            else{
                return false;
            }
        }
        else if(tokenState == TOKEN_STATE_ID){
            if(IsLiteral(currentInput[i])){
                inputToken.push_back(ID);
                inputToken.push_back(currentInput[i]);
                tokenState = TOKEN_STATE_WHITE;
            }
            else if(currentInput[i] == ' '){
                inputToken.push_back(ID);
                tokenState = TOKEN_STATE_WHITE;
            }
            else if(IsLetter(currentInput[i])){
                //do nothing
            }
            else if(IsDigit(currentInput[i])){
                //do nothing
            }
            else{
                return false;
            }
        }
        else if(tokenState == TOKEN_STATE_INT){
            if(IsLiteral(currentInput[i])){
                inputToken.push_back(INT);
                inputToken.push_back(currentInput[i]);
                tokenState = TOKEN_STATE_WHITE;
            }
            else if(currentInput[i] == ' '){
                inputToken.push_back(INT);
                tokenState = TOKEN_STATE_WHITE;
            }
            else if(currentInput[i] == '.'){
                tokenState = TOKEN_STATE_REAL;
            }
            else if(IsDigit(currentInput[i])){
                //do nothing
            }
            else{
                return false;
            }
        }
        else if(tokenState == TOKEN_STATE_REAL){
            if(IsLiteral(currentInput[i])){
                inputToken.push_back(REAL);
                inputToken.push_back(currentInput[i]);
                tokenState = TOKEN_STATE_WHITE;
            }
            else if(currentInput[i] == ' '){
                inputToken.push_back(REAL);
                tokenState = TOKEN_STATE_WHITE;
            }
            else if(IsDigit(currentInput[i])){
                //do nothing
            }
            else{
                return false;
            }
        }
    }

    if(tokenState == TOKEN_STATE_ID){
        inputToken.push_back(ID);
    }
    else if(tokenState == TOKEN_STATE_INT){
        inputToken.push_back(INT);
    }
    else if(tokenState == TOKEN_STATE_REAL){
        inputToken.push_back(REAL);
    }

    return true;
}

bool IsLiteral(char input){
    if(input == '+' | input == '-' | input == '*' | input == '/'
       | input == '(' | input == ')'| input == '='  | input == ';') return true;
    else return false;
}

bool IsLetter(char input){
    if((input >= 'a' && input <= 'z') | (input >= 'A' && input <= 'Z')) return true;
    else return false;
}

bool IsDigit(char input){
    if((input >= '0' && input <= '9')) return true;
    else return false;
}

bool Parser(){
    parseStack.push_front(S);
    parseState = PARSE_STATE_WAIT;
    while(true){
        //PrintList();
        //PrintStack();
        if(parseState == PARSE_STATE_WAIT){
            if(inputToken.empty()) parseState = PARSE_STATE_EOL;
            else{
                if(inputToken.front() == ID) parseState = PARSE_STATE_ID;
                else if(inputToken.front() == INT) parseState = PARSE_STATE_INT;
                else if(inputToken.front() == REAL) parseState = PARSE_STATE_REAL;
                else if(inputToken.front() == '+') parseState = PARSE_STATE_PLUS;
                else if(inputToken.front() == '-') parseState = PARSE_STATE_MINUS;
                else if(inputToken.front() == '*') parseState = PARSE_STATE_MULTI;
                else if(inputToken.front() == '/') parseState = PARSE_STATE_DEVIDE;
                else if(inputToken.front() == '(') parseState = PARSE_STATE_OPEN_B;
                else if(inputToken.front() == ')') parseState = PARSE_STATE_CLOSE_B;
                else if(inputToken.front() == '=') parseState = PARSE_STATE_EQUAL;
                else if(inputToken.front() == ';') parseState = PARSE_STATE_SEMI_C;
                else break;
                inputToken.pop_front();
            }
        }
        else if(parseState == PARSE_STATE_ID){
            if(parseStack.front() == ID){
                parseStack.pop_front();
                parseState = PARSE_STATE_WAIT;
            }
            else if(parseStack.front() == S){
                parseStack.pop_front();
                parseStack.push_front(S);
                parseStack.push_front(Z);
            }
            else if(parseStack.front() == Z){
                parseStack.pop_front();
                parseStack.push_front(';');
                parseStack.push_front(E);
                parseStack.push_front('=');
                parseStack.push_front(ID);
            }
            else if(parseStack.front() == E){
                parseStack.pop_front();
                parseStack.push_front(E_);
                parseStack.push_front(T);
            }
            else if(parseStack.front() == E_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == T){
                parseStack.pop_front();
                parseStack.push_front(T_);
                parseStack.push_front(F);
            }
            else if(parseStack.front() == T_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == F){
                parseStack.pop_front();
                parseStack.push_front(A);
                parseStack.push_front(ID);
            }
            else if(parseStack.front() == A){
                parseStack.pop_front();
            }
            else break;
        }
        else if(parseState == PARSE_STATE_INT){
            if(parseStack.front() == INT){
                parseStack.pop_front();
                parseState = PARSE_STATE_WAIT;
            }
            else if(parseStack.front() == S){
                parseStack.pop_front();
                parseStack.push_front(S);
                parseStack.push_front(Z);
            }
            else if(parseStack.front() == E){
                parseStack.pop_front();
                parseStack.push_front(E_);
                parseStack.push_front(T);
            }
            else if(parseStack.front() == E_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == T){
                parseStack.pop_front();
                parseStack.push_front(T_);
                parseStack.push_front(F);
            }
            else if(parseStack.front() == T_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == F){
                parseStack.pop_front();
                parseStack.push_front(INT);
            }
            else if(parseStack.front() == A){
                parseStack.pop_front();
            }
            else break;
        }
        else if(parseState == PARSE_STATE_REAL){
            if(parseStack.front() == REAL){
                parseStack.pop_front();
                parseState = PARSE_STATE_WAIT;
            }
            else if(parseStack.front() == S){
                parseStack.pop_front();
                parseStack.push_front(S);
                parseStack.push_front(Z);
            }
            else if(parseStack.front() == E){
                parseStack.pop_front();
                parseStack.push_front(E_);
                parseStack.push_front(T);
            }
            else if(parseStack.front() == E_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == T){
                parseStack.pop_front();
                parseStack.push_front(T_);
                parseStack.push_front(F);
            }
            else if(parseStack.front() == T_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == F){
                parseStack.pop_front();
                parseStack.push_front(REAL);
            }
            else if(parseStack.front() == A){
                parseStack.pop_front();
            }
            else break;
        }
        else if(parseState == PARSE_STATE_PLUS){
            if(parseStack.front() == '+'){
                parseStack.pop_front();
                parseState = PARSE_STATE_WAIT;
            }
            else if(parseStack.front() == S){
                parseStack.pop_front();
                parseStack.push_front(S);
                parseStack.push_front(Z);
            }
            else if(parseStack.front() == E){
                parseStack.pop_front();
                parseStack.push_front(E_);
                parseStack.push_front(T);
            }
            else if(parseStack.front() == E_){
                parseStack.pop_front();
                parseStack.push_front(E_);
                parseStack.push_front(T);
                parseStack.push_front('+');
            }
            else if(parseStack.front() == T){
                parseStack.pop_front();
                parseStack.push_front(T_);
                parseStack.push_front(F);
            }
            else if(parseStack.front() == T_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == A){
                parseStack.pop_front();
            }
            else break;
        }
        else if(parseState == PARSE_STATE_MINUS){
            if(parseStack.front() == '-'){
                parseStack.pop_front();
                parseState = PARSE_STATE_WAIT;
            }
            else if(parseStack.front() == S){
                parseStack.pop_front();
                parseStack.push_front(S);
                parseStack.push_front(Z);
            }
            else if(parseStack.front() == E){
                parseStack.pop_front();
                parseStack.push_front(E_);
                parseStack.push_front(T);
            }
            else if(parseStack.front() == E_){
                parseStack.pop_front();
                parseStack.push_front(E_);
                parseStack.push_front(T);
                parseStack.push_front('-');
            }
            else if(parseStack.front() == T){
                parseStack.pop_front();
                parseStack.push_front(T_);
                parseStack.push_front(F);
            }
            else if(parseStack.front() == T_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == A){
                parseStack.pop_front();
            }
            else break;
        }
        else if(parseState == PARSE_STATE_MULTI){
            if(parseStack.front() == '*'){
                parseStack.pop_front();
                parseState = PARSE_STATE_WAIT;
            }
            else if(parseStack.front() == S){
                parseStack.pop_front();
                parseStack.push_front(S);
                parseStack.push_front(Z);
            }
            else if(parseStack.front() == E){
                parseStack.pop_front();
                parseStack.push_front(E_);
                parseStack.push_front(T);
            }
            else if(parseStack.front() == E_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == T){
                parseStack.pop_front();
                parseStack.push_front(T_);
                parseStack.push_front(F);
            }
            else if(parseStack.front() == T_){
                parseStack.pop_front();
                parseStack.push_front(T_);
                parseStack.push_front(F);
                parseStack.push_front('*');
            }
            else if(parseStack.front() == A){
                parseStack.pop_front();
            }
            else break;
        }
        else if(parseState == PARSE_STATE_DEVIDE){
            if(parseStack.front() == '/'){
                parseStack.pop_front();
                parseState = PARSE_STATE_WAIT;
            }
            else if(parseStack.front() == S){
                parseStack.pop_front();
                parseStack.push_front(S);
                parseStack.push_front(Z);
            }
            else if(parseStack.front() == E){
                parseStack.pop_front();
                parseStack.push_front(E_);
                parseStack.push_front(T);
            }
            else if(parseStack.front() == E_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == T){
                parseStack.pop_front();
                parseStack.push_front(T_);
                parseStack.push_front(F);
            }
            else if(parseStack.front() == T_){
                parseStack.pop_front();
                parseStack.push_front(T_);
                parseStack.push_front(F);
                parseStack.push_front('/');
            }
            else if(parseStack.front() == A){
                parseStack.pop_front();
            }
            else break;
        }
        else if(parseState == PARSE_STATE_OPEN_B){
            if(parseStack.front() == '('){
                parseStack.pop_front();
                parseState = PARSE_STATE_WAIT;
            }
            else if(parseStack.front() == S){
                parseStack.pop_front();
                parseStack.push_front(S);
                parseStack.push_front(Z);
            }
            else if(parseStack.front() == E){
                parseStack.pop_front();
                parseStack.push_front(E_);
                parseStack.push_front(T);
            }
            else if(parseStack.front() == E_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == T){
                parseStack.pop_front();
                parseStack.push_front(T_);
                parseStack.push_front(F);
            }
            else if(parseStack.front() == T_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == F){
                parseStack.pop_front();
                parseStack.push_front(')');
                parseStack.push_front(E);
                parseStack.push_front('(');
            }
            else if(parseStack.front() == A){
                parseStack.pop_front();
                parseStack.push_front(')');
                parseStack.push_front(E);
                parseStack.push_front('(');
            }
            else break;
        }
        else if(parseState == PARSE_STATE_CLOSE_B){
            if(parseStack.front() == ')'){
                parseStack.pop_front();
                parseState = PARSE_STATE_WAIT;
            }
            else if(parseStack.front() == S){
                parseStack.pop_front();
                parseStack.push_front(S);
                parseStack.push_front(Z);
            }
            else if(parseStack.front() == E){
                parseStack.pop_front();
                parseStack.push_front(E_);
                parseStack.push_front(T);
            }
            else if(parseStack.front() == E_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == T){
                parseStack.pop_front();
                parseStack.push_front(T_);
                parseStack.push_front(F);
            }
            else if(parseStack.front() == T_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == A){
                parseStack.pop_front();
            }
            else break;
        }
        else if(parseState == PARSE_STATE_EQUAL){
            if(parseStack.front() == '='){
                parseStack.pop_front();
                parseState = PARSE_STATE_WAIT;
            }
            else if(parseStack.front() == S){
                parseStack.pop_front();
                parseStack.push_front(S);
                parseStack.push_front(Z);
            }
            else if(parseStack.front() == E){
                parseStack.pop_front();
                parseStack.push_front(E_);
                parseStack.push_front(T);
            }
            else if(parseStack.front() == E_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == T){
                parseStack.pop_front();
                parseStack.push_front(T_);
                parseStack.push_front(F);
            }
            else if(parseStack.front() == T_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == A){
                parseStack.pop_front();
            }
            else break;
        }
        else if(parseState == PARSE_STATE_SEMI_C){
            if(parseStack.front() == ';'){
                parseStack.pop_front();
                parseState = PARSE_STATE_WAIT;
            }
            else if(parseStack.front() == S){
                parseStack.pop_front();
                parseStack.push_front(S);
                parseStack.push_front(Z);
            }
            else if(parseStack.front() == E){
                parseStack.pop_front();
                parseStack.push_front(E_);
                parseStack.push_front(T);
            }
            else if(parseStack.front() == E_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == T){
                parseStack.pop_front();
                parseStack.push_front(T_);
                parseStack.push_front(F);
            }
            else if(parseStack.front() == T_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == A){
                parseStack.pop_front();
            }
            else break;
        }
        else if(parseState == PARSE_STATE_EOL){
            if(parseStack.front() == S){
                parseStack.pop_front();
            }
            else if(parseStack.front() == E){
                parseStack.pop_front();
                parseStack.push_front(E_);
                parseStack.push_front(T);
            }
            else if(parseStack.front() == E_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == T){
                parseStack.pop_front();
                parseStack.push_front(T_);
                parseStack.push_front(F);
            }
            else if(parseStack.front() == T_){
                parseStack.pop_front();
            }
            else if(parseStack.front() == A){
                parseStack.pop_front();
            }
            else break;
        }
        else {
            printf(">>>>>CRITICAL ERROR<<<<<\n");
            break;
        }
    }
    if(parseStack.empty() && inputToken.empty() && parseState == PARSE_STATE_EOL)
        return true;
    else return false;
}

void PrintList(){
    printf(">>> ");
    list<char>::iterator iter = inputToken.begin();
    while(iter != inputToken.end()){
        if(*iter == ID) printf("ID ");
        else if(*iter == INT) printf("INT ");
        else if(*iter == REAL) printf("REAL ");
        else printf("%c ", *iter);
        iter++;
    }
    printf("\n");
}

void PrintStack(){
    printf(">> ");
    list<char>::iterator iter = parseStack.begin();
    while(iter != parseStack.end()){
        if(*iter == ID) printf("ID ");
        else if(*iter == INT) printf("INT ");
        else if(*iter == REAL) printf("REAL ");
        else if(*iter == S) printf("S ");
        else if(*iter == Z) printf("Z ");
        else if(*iter == E) printf("E ");
        else if(*iter == E_) printf("E_ ");
        else if(*iter == T) printf("T ");
        else if(*iter == T_) printf("T_ ");
        else if(*iter == F) printf("F ");
        else if(*iter == A) printf("A ");
        else printf("%c ", *iter);
        iter++;
    }
    printf("\n");
}
