#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_SIZE 100

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_SIZE];
} Token;

char* input;
int pos = 0;

Token getNextToken() {
    Token token;
    token.type = TOKEN_UNKNOWN;
    memset(token.value, 0, MAX_TOKEN_SIZE);

    // Skip whitespace
    while (isspace(input[pos])) {
        pos++;
    }

    // Check for end of input
    if (input[pos] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    // Check for identifier (alphanumeric starting with a letter)
    if (isalpha(input[pos])) {
        int start = pos;
        while (isalnum(input[pos])) {
            pos++;
        }
        strncpy(token.value, &input[start], pos - start);
        token.type = TOKEN_IDENTIFIER;
        return token;
    }

    // Check for number (sequence of digits)
    if (isdigit(input[pos])) {
        int start = pos;
        while (isdigit(input[pos])) {
            pos++;
        }
        strncpy(token.value, &input[start], pos - start);
        token.type = TOKEN_NUMBER;
        return token;
    }

    // Check for operators (e.g., +, -, *, /)
    if (strchr("+-*/", input[pos]) != NULL) {
        token.value[0] = input[pos];
        token.type = TOKEN_OPERATOR;
        pos++;
        return token;
    }

    // If none of the above, return the current character as an unknown token
    token.value[0] = input[pos];
    pos++;
    return token;
}

int main() {
    input = "x = 42 + 7 * y";

    Token token;
    do {
        token = getNextToken();
        switch (token.type) {
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER: %s\n", token.value);
                break;
            case TOKEN_NUMBER:
                printf("NUMBER: %s\n", token.value);
                break;
            case TOKEN_OPERATOR:
                printf("OPERATOR: %s\n", token.value);
                break;
            case TOKEN_UNKNOWN:
                printf("UNKNOWN: %s\n", token.value);
                break;
            case TOKEN_EOF:
                printf("EOF\n");
                break;
        }
    } while (token.type != TOKEN_EOF);

    return 0;
}
