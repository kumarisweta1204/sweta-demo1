#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Token types
typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,  // '('
    TOKEN_RPAREN,  // ')'
    TOKEN_EOF,
    TOKEN_INVALID
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    int value;  // Used only if the token is a number
} Token;

// AST Node types
typedef enum {
    AST_NUMBER,
    AST_BINARY_OP
} ASTNodeType;

// AST Node structure
typedef struct ASTNode {
    ASTNodeType type;
    int value;  // Used if the node is a number
    TokenType op;  // Operator, used if the node is a binary operation
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

char* input;
int pos = 0;

Token getNextToken() {
    Token token;
    while (isspace(input[pos])) pos++;  // Skip whitespace

    if (isdigit(input[pos])) {
        token.type = TOKEN_NUMBER;
        token.value = 0;
        while (isdigit(input[pos])) {
            token.value = token.value * 10 + (input[pos] - '0');
            pos++;
        }
        return token;
    }

    switch (input[pos]) {
        case '+': token.type = TOKEN_PLUS; break;
        case '-': token.type = TOKEN_MINUS; break;
        case '*': token.type = TOKEN_MULTIPLY; break;
        case '/': token.type = TOKEN_DIVIDE; break;
        case '(': token.type = TOKEN_LPAREN; break;
        case ')': token.type = TOKEN_RPAREN; break;
        case '\0': token.type = TOKEN_EOF; break;
        default: token.type = TOKEN_INVALID; break;
    }
    pos++;
    return token;
}

// expr   -> term ((PLUS | MINUS) term)*
// term   -> factor ((MUL | DIV) factor)*
// factor -> NUMBER | LPAREN expr RPAREN


Token currentToken;

void advance() {
    currentToken = getNextToken();
}

ASTNode* createASTNode(ASTNodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->left = node->right = NULL;
    return node;
}

// Forward declarations
ASTNode* expr();
ASTNode* term();
ASTNode* factor();

ASTNode* expr() {
    ASTNode* node = term();

    while (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS) {
        TokenType op = currentToken.type;
        advance();
        ASTNode* newNode = createASTNode(AST_BINARY_OP);
        newNode->op = op;
        newNode->left = node;
        newNode->right = term();
        node = newNode;
    }

    return node;
}

ASTNode* term() {
    ASTNode* node = factor();

    while (currentToken.type == TOKEN_MULTIPLY || currentToken.type == TOKEN_DIVIDE) {
        TokenType op = currentToken.type;
        advance();
        ASTNode* newNode = createASTNode(AST_BINARY_OP);
        newNode->op = op;
        newNode->left = node;
        newNode->right = factor();
        node = newNode;
    }

    return node;
}

ASTNode* factor() {
    ASTNode* node;

    if (currentToken.type == TOKEN_NUMBER) {
        node = createASTNode(AST_NUMBER);
        node->value = currentToken.value;
        advance();
    } else if (currentToken.type == TOKEN_LPAREN) {
        advance();  // skip '('
        node = expr();
        if (currentToken.type != TOKEN_RPAREN) {
            fprintf(stderr, "Expected ')'\n");
            exit(1);
        }
        advance();  // skip ')'
    } else {
        fprintf(stderr, "Unexpected token\n");
        exit(1);
    }

    return node;
}
