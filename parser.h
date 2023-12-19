#pragma once
#include <iostream>
#include <string>
#include <stack>
#include "lexer.h"
#include "token.h"
#include "node.h"

class parser
{
public:
    parser(const std::vector<token>& tokens) : tokens(tokens), currentTokenIndex(0) {}
    ASTNode* parse() {
        std::vector<ASTNode*> statements;

        while (currentTokenIndex < tokens.size()) {
            statements.push_back(parseStatements());
        }

        return new ProgramNode(statements);
    }

private:
    const std::vector<token>& tokens;
    size_t currentTokenIndex;


    // CAN BE FINALIZED
    ASTNode* parseStatements() {
        if (get_token_type() == token_type::DIRECTIVE_INCLUDE) {
            return preprocessorDirectiveParse();
        }
        else if (get_token_type() == token_type::INT || get_token_type() == token_type::DOUBLE) {
            if (get_token_type_offset(2) == token_type::LPAR && get_token_type_offset(1) == token_type::IDENTIFIER) {
                return functionDeclarationParse();
            }
            else if (get_token_type_offset(2) == token_type::ASSIGN || get_token_type_offset(2) == token_type::SEMICOLON) {
                return variableDeclarationParse();
            }
            else {
                error_handle::raise(error_handle_type::PARSER, "Identificator expected.");
                return nullptr;
            }
        }
        else {
            error_handle::raise(error_handle_type::PARSER, "Must be inside the function.");
            return nullptr;
        }
    }
    // READY
    ASTNode* preprocessorDirectiveParse() {
        std::string fileName;
        next_token();
        if (get_token_type() != token_type::LESS) {
            error_handle::raise(error_handle_type::PARSER, "Expected file name in <>");
            return nullptr;
        }
        next_token();
        fileName = get_token_lexeme();
        next_token();

        if (get_token_type() != token_type::GREATER) {
            error_handle::raise(error_handle_type::PARSER, "Expected file name in <>");
            return nullptr;
        }
        next_token();
        return new DerectivIncludeNode(fileName);
    }

    // READY
    ASTNode* expressionParse() {
        std::stack<ASTNode*> operands;
        std::stack<token_type> operators;

        ASTNode* currentOperand = primaryExpressionParse();
        operands.push(currentOperand);

        while (is_binary_operator(get_token_type())) {
            token_type currentOperator = get_token_type();

            while (!operators.empty() && get_operator_precedence(operators.top()) >= get_operator_precedence(currentOperator)) {

                ASTNode* rightOperand = operands.top();
                operands.pop();

                ASTNode* leftOperand = operands.top();
                operands.pop();

                ASTNode* newExpression = new BinaryOperationNode(leftOperand, operators.top(), rightOperand);
                operands.push(newExpression);

                operators.pop();
            }

            operators.push(currentOperator);
            next_token();

            currentOperand = primaryExpressionParse();
            operands.push(currentOperand);
        }

        while (!operators.empty()) {
            ASTNode* rightOperand = operands.top();
            operands.pop();

            ASTNode* leftOperand = operands.top();
            operands.pop();

            ASTNode* newExpression = new BinaryOperationNode(leftOperand, operators.top(), rightOperand);
            operands.push(newExpression);

            operators.pop();
        }

        return operands.top();
    }
    // READY
    ASTNode* primaryExpressionParse() {
        token_type currentTokenType = get_token_type();

        if (currentTokenType == token_type::IDENTIFIER) {
            std::string identifier = get_token_lexeme();
            if (get_token_type() != token_type::ASSIGN) {
                next_token();
            }
            return new IdentifierNode(identifier);
        }

        if (currentTokenType == token_type::INTEGER_CONST || currentTokenType == token_type::DOUBLE_CONST) {
            std::string value = get_token_lexeme();
            next_token();
            if (get_token_type() == token_type::SEMICOLON) {
                next_token();
            }
            return new LiteralNode(value);
        }

        if (currentTokenType == token_type::LPAR) {
            next_token();
            ASTNode* expressionInsideParentheses = expressionParse();

            if (get_token_type() != token_type::RPAR) {
                error_handle::raise(error_handle_type::PARSER, "Expected closing parenthesis.");
                return nullptr;
            }

            next_token();
            return expressionInsideParentheses;
        }

        error_handle::raise(error_handle_type::PARSER, "An identifier, constant, or expression in parentheses is expected.");
        return nullptr;
    }
    // READY
    ASTNode* variableDeclarationParse() {
        if (get_token_type() == token_type::INT || get_token_type() == token_type::DOUBLE) {
            next_token();
        }

        if (get_token_type() != token_type::IDENTIFIER) {
            error_handle::raise(error_handle_type::PARSER, "Expected variable identifier after type.");
            return nullptr;
        }
        std::string variableIdentifire = get_token_lexeme();
        next_token();

        if (get_token_type() == token_type::ASSIGN) {
            next_token();
            ASTNode* expression = expressionParse();
            return new VariableDeclarationNode(variableIdentifire, expression);
        }
        return new VariableDeclarationNode(variableIdentifire, nullptr);

    }

    //READY
    ASTNode* functionParametersParse() {
        std::vector<ASTNode*> statements;

        if (get_token_type() != token_type::LPAR) {
            error_handle::raise(error_handle_type::PARSER, "The left bracket is expected.");
            return nullptr;
        }

        next_token();

        if (get_token_type() == token_type::RPAR) {
            return nullptr;
        }

        statements.push_back(variableDeclarationParse());
        if (get_token_type() != token_type::RPAR && get_token_type() != token_type::COMMA) {
            error_handle::raise(error_handle_type::PARSER, "Check the arguments of the function.");
            return nullptr;
        }

        while (get_token_type() == token_type::COMMA) {
            next_token();
            statements.push_back(variableDeclarationParse());
        }

        if (get_token_type() != token_type::RPAR) {
            error_handle::raise(error_handle_type::PARSER, "The right bracket is expected.");
            return nullptr;
        }

        return new ParameterListNode(statements);
    }
    // READY
    ASTNode* compoundStatementsParse() {
        if (get_token_type() != token_type::LBRA) {
            error_handle::raise(error_handle_type::PARSER, "An left brace is expected.");
            return nullptr;
        }

        next_token();
        std::vector<ASTNode*> statements;
        while (get_token_type() != token_type::RBRA) {
            ASTNode* statement = compaundParse();
            if (statement != nullptr) {
                statements.push_back(statement);
                if (get_token_type() == token_type::SEMICOLON) {
                    next_token();
                }
            }
        }

        if (get_token_type() != token_type::RBRA) {
            error_handle::raise(error_handle_type::PARSER, "Expected closing curly brace for compound statement.");
            return nullptr;
        }
        next_token();
        if (statements.empty()) {
            error_handle::raise(error_handle_type::PARSER, "Compound can't be empty.");
        }
        return new CompoundStatementNode(statements);
    }
    // READY
    ASTNode* compaundParse() {
        token_type currentTokenType = get_token_type();
        if (currentTokenType == token_type::IDENTIFIER && get_token_type_offset(1) == token_type::SEMICOLON) {
            error_handle::raise(error_handle_type::PARSER, "The identifier cannot be used alone.");
            return nullptr;
        }

        if (currentTokenType == token_type::INT || currentTokenType == token_type::DOUBLE || currentTokenType == token_type::VOID) {

            return variableDeclarationParse();
        }
        else if (currentTokenType == token_type::IF || currentTokenType == token_type::ELSE) {

            return parseIfElseExpression();
        }
        else if (currentTokenType == token_type::RETURN){
            return parseReturnStatement(); 
        }
        else {

            return expressionParse();
        }
    }
    // READY
    ASTNode* functionDeclarationParse() {
        token_type returnType = get_token_type();

        next_token();
        std::string name = get_token_lexeme();

        next_token();
        ASTNode* parameters = functionParametersParse();

        next_token();
        ASTNode* compounds = compoundStatementsParse();
        return new FunctionDeclarationNode(parameters, compounds, name, returnType);
    }
    // READY
    ASTNode* parseIfElseExpression() {
        next_token();

        if (get_token_type() != token_type::LPAR) {
            error_handle::raise(error_handle_type::PARSER, "Expected opening parenthesis after if/else.");
            return nullptr;
        }

        next_token();
        if (get_token_type() == token_type::RPAR) {
            error_handle::raise(error_handle_type::PARSER, "Condition can't be empty.");
            return nullptr;
        }
        ASTNode* condition = expressionParse();

        if (get_token_type() != token_type::RPAR) {
            error_handle::raise(error_handle_type::PARSER, "Expected closing parenthesis after if/else condition.");
            return nullptr;
        }

        next_token();
        ASTNode* codeBlock = compoundStatementsParse();

        if (get_token_type() == token_type::ELSE) {
            next_token();
            ASTNode* elseBlock = compoundStatementsParse();
            return new IFElseNode(condition, codeBlock, elseBlock);
        }

        return new IfNode(condition, codeBlock);
    }

    ASTNode* parseReturnStatement(){
        next_token();
        if (get_token_type() != token_type::IDENTIFIER && get_token_type() != token_type::INTEGER_CONST && get_token_type() != token_type::DOUBLE_CONST){
            error_handle::raise(error_handle_type::PARSER, "Probably the return value.");
        }
        ASTNode* expression = expressionParse();
        return new ReturnStatementNode(expression);
    }
    // READY
    ASTNode* assignmentStatementParse() {

        std::string variableIdentifier = get_token_lexeme_offset(-1);
        if (get_token_type() == token_type::ASSIGN) {
            next_token();


            ASTNode* expression = expressionParse();
            return new AssignmentStatementNode(new IdentifierNode(variableIdentifier), expression);
        }
        else {
            error_handle::raise(error_handle_type::PARSER, "Expected assignment operator.");
            return nullptr;
        }

    }

    token_type get_token_type() {
        return tokens[currentTokenIndex].type;
    }

    std::string get_token_lexeme() {
        return tokens[currentTokenIndex].lexeme;
    }

    std::string get_token_lexeme_offset(int offset) {
        return tokens[currentTokenIndex + offset].lexeme;
    }

    token_type get_token_type_offset(int offset) {
        return tokens[currentTokenIndex + offset].type;
    }

    void next_token() {
        currentTokenIndex++;
    }

    bool is_binary_operator(token_type type) {
        return type == token_type::PLUS || type == token_type::MINUS ||
            type == token_type::STAR || type == token_type::SLASH ||
            type == token_type::EQUAL || type == token_type::LESS ||
            type == token_type::GREATER || type == token_type::LESS_EQUAL ||
            type == token_type::GREATER_EQUAL || type == token_type::NOT_EQUAL ||
            type == token_type::AND || type == token_type::OR ||
            type == token_type::ASSIGN;
    }

    bool is_digit(token_type type) {
        return type == token_type::INT || type == token_type::DOUBLE;
    }

    bool is_function(token_type type) {
        return type == token_type::LPAR;
    }

    int get_operator_precedence(token_type type) {
        switch (type) {
        case token_type::ASSIGN:
            return 1;
        case token_type::OR:
            return 2;
        case token_type::AND:
            return 3;
        case token_type::EQUAL:
        case token_type::NOT_EQUAL:
        case token_type::LESS:
        case token_type::LESS_EQUAL:
        case token_type::GREATER:
        case token_type::GREATER_EQUAL:
            return 4;
        case token_type::PLUS:
        case token_type::MINUS:
            return 5;
        case token_type::STAR:
        case token_type::SLASH:
            return 6;
        default:
            return 0;
        }
    }
};
