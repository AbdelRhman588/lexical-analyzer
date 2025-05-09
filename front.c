﻿/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <ctype.h>

/* Global declarations */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

/*/
/* main driver */
int main() {
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("front.in", "r")) == NULL)
        printf("ERROR - cannot open front.in \n");
    else {
        getChar();
        do {
            lex();
        } while (nextToken != EOF);
    }
    return 0;
}

/*/
/* lookup - a function to lookup operators and parentheses and return the token */
int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        default:
            addChar();
            nextToken = -2; // Invalid token
            printf("Error: Invalid character '%c'\n", ch);
            break;
    }
    return nextToken;
}

/*/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else
        printf("Error - lexeme is too long \n");
}

/*/
/* getChar - a function to get the next character of input and determine its character class */
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    } else
        charClass = EOF;
}

/*/
/* getNonBlank - a function to call getChar until it returns a non-whitespace character */
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

/*/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;
        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } /* End of switch */
    
    /* Print the token in a more readable format */
    if (nextToken != -2) { // Skip printing for invalid tokens (already handled in lookup)
        switch (nextToken) {
            case IDENT:
                printf("Token: IDENTIFIER, Lexeme: %s\n", lexeme);
                break;
            case INT_LIT:
                printf("Token: INTEGER, Lexeme: %s\n", lexeme);
                break;
            case ADD_OP:
                printf("Token: ADD_OP, Lexeme: %s\n", lexeme);
                break;
            case SUB_OP:
                printf("Token: SUB_OP, Lexeme: %s\n", lexeme);
                break;
            case MULT_OP:
                printf("Token: MULT_OP, Lexeme: %s\n", lexeme);
                break;
            case DIV_OP:
                printf("Token: DIV_OP, Lexeme: %s\n", lexeme);
                break;
            case LEFT_PAREN:
                printf("Token: LEFT_PAREN, Lexeme: %s\n", lexeme);
                break;
            case RIGHT_PAREN:
                printf("Token: RIGHT_PAREN, Lexeme: %s\n", lexeme);
                break;
            case EOF:
                printf("Token: EOF, Lexeme: %s\n", lexeme);
                break;
            default:
                printf("Token: UNKNOWN, Lexeme: %s\n", lexeme);
                break;
        }
    }
    return nextToken;
} /* End of function lex */