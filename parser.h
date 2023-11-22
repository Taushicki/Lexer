#pragma once
#include <iostream>
#include <string>
#include "lexer.h"
#include "token.h"


enum class ASTNodeType {
    IDENTIFIER_NODE,

    INTENGER_CONST_NODE,
    DOUBLE_CONST_NODE,
    STRING_CONST_NODE,
    CHAR_CONST_NODE,
    BOOLEAN_CONST_NODE,

    VARIABLEDECLARATION_NODE,

    WHILE_LOOP_NODE,
    DO_WHILE_LOOP_NODE,
    FOR_LOOP_NODE,

    BREAK_STATEMENT_NODE,
    CONTINUE_STATEMENT_NODE,


    SWITHC_STATEMENT_NODE,
    CASE_NODE,
    DEFAULT_NODE,

    IF_STATEMENT_NODE,
    ELSE_STATEMENT_NODE,

    FUNCTION_DECLARATION_NODE,
    RETURN_STATEMENT_NODE,

    BINARY_OPERATION_NODE,
    UNARY_OPERATION_NODE,
    ASSIGMENT_NODE,
    TERNARY_CONDITIONAL_NODE,

    LINE_COMMENT_NODE,
    BLOCK_COMMENT_NODE,

    NEW_EXPRESSION_NODE,
    DELETE_EXPRESSION_NODE,

    PREPROCESSOR_DIRECTIVE_NODE,

    ACCESS_OPERATOR_NODE
};

class ASTNode 
{
public:
    ASTNode(ASTNodeType type, const std::string& value)
        : type(type), value(value) {}

    // ... ������ ������ � ���� �� ���� ������������� ...

private:
    ASTNodeType type;
    std::string value;
    std::vector<ASTNode> children;
};

class parser 
{
public:
    parser(const std::vector<token>& tokens) : tokens(tokens), currentTokenIndex(0) {}

    ASTNode parse() 
    {
        // ���������� ������ �������
        // ��������, ����� ������������ ����� parseExpression ��� ������
        return parse_expression();
    }

private:
    const std::vector<token>& tokens;
    size_t currentTokenIndex;

    ASTNode parse_expression()
    {
        // ��� ��� ��� ������� ���������
        // ��������:
        ASTNode left = parse_term();

        while (current_token().type == token_type::PLUS || 
            current_token().type == token_type::MINUS ||
            current_token().type == token_type::STAR ||
            current_token().type == token_type::SLASH)
        {
            token_type operationType = current_token().type;
            next_token(); // ���������� ��������
            ASTNode right = parse_term();
            // ������� ���� ��� ��������
            ASTNode operationNode = ASTNode(get_operation_type(operationType), "");
            // ��������� ����
            operationNode.add_child(left);
            operationNode.add_child(right);
            left = operationNode;
        }

        return left;
    }

    ASTNode parse_term() 
    {
        // ��� ��� ��� ������� ������
    }

    // ������ ������ �� ���� �������������...

    token current_token() 
    {
        return tokens[currentTokenIndex];
    }

    void next_token() 
    {
        currentTokenIndex++;
    }

    // ������ �������������� ���� �������� �� TokenType � ASTNodeType
    ASTNodeType get_operation_type(token_type token_type) 
    {
        // ��� ��� ��������������
    }
};
