#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "lexer.h"

void token_next(Token *t)
{
    static bool can_be_unary = true;
    char c;
    
    do {
        c = fgetc(stdin);
    } while (isspace(c));
    
    if (c == EOF) {
        t->type = FINAL;
    }
    
    else if (isalpha(c) || c == '_') {
        t->type = VARIABLE;
        t->data.variable_name = c;
        can_be_unary = false;
    }
    
    else if (isdigit(c)) {
        float result;
        ungetc(c, stdin);
        scanf("%f", &result);
        
        if (result == (int) result) {
            t->type = INTEGER;
            t->data.value_int = (int) result;
        } else {
            t->type = FLOATING;
            t->data.value_float = result;
        }
        can_be_unary = false;
    }
    
    else if (c == '(' || c == ')') {
        t->type = BRACKET;
        t->data.is_left_bracket = (c == '(');
        can_be_unary = t->data.is_left_bracket;
    }
    
    else if (can_be_unary && (c == '-' || c == '+')) {
        int m = (c == '+') ? +1 : -1;
        
        do {
            c = fgetc(stdin);
        } while (isspace(c));
        
        if (isdigit(c)) {
            ungetc(c, stdin);
            token_next(t);
            if (t->type == INTEGER) {
                t->data.value_int = m * (t->data.value_int);
            } else {
                t->data.value_float = m * (t->data.value_float);
            }
        } else {
            ungetc(c, stdin);
            t->type = OPERATOR;
            t->data.operator_name = '-';
            can_be_unary = true;
        }
    }
    
    else {
        t->type = OPERATOR;
        t->data.operator_name = c;
        can_be_unary = true;
    }
}

void token_print(Token *t)
{
    switch (t->type) {
        case FINAL:
            break;
        case INTEGER:
            printf("%d", t->data.value_int);
            break;
        case FLOATING:
            printf("%lg", t->data.value_float);
            break;
        case VARIABLE:
            printf("%c", t->data.variable_name);
            break;
        case OPERATOR:
            printf("%c", t->data.operator_name);
            break;
        case BRACKET:
            printf("%c", (t->data.is_left_bracket) ? '(' : ')');
            break;
    }
}