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
    parser(const std::vector<token>& tokens) : tokens(tokens), currentTokenIndex(0) {} // Передаём вектор с токенами
    ASTNode* parse() 
    {
        std::vector<ASTNode*> statements; // Создаём вектор указателей на узлы

        while (currentTokenIndex < tokens.size()) { //Крутимся до тех пор, пока не закончится вектор токенов
            statements.push_back(parseStatements()); // На каждой итерации помещем в вектор с узлами новый узел
        }

        return new ProgramNode(statements); // В конце получаем ProgramNode который хранит в себе все остальные узлы
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
                error_handle::raise(error_handle_type::PARSER, "Identificator expected");
                return nullptr;
            }
        }
        else {
            error_handle::raise(error_handle_type::PARSER, "Must be inside the function!");
            return nullptr;
        }
        
    }
    // CAN BE FINALIZED
    ASTNode* preprocessorDirectiveParse() {
        std::string fileName;
        next_token();
        if (get_token_type() == token_type::LESS && get_token_type_offset(2) == token_type::GREATER) {
            next_token();
            fileName = get_token_lexeme();
            next_token();
            next_token();
            return new DerectivIncludeNode(fileName);
        }
        else if (get_token_type() == token_type::STRING_CONST) {
            next_token();
            fileName = get_token_lexeme().substr(1, fileName.size() - 2);
            return new DerectivIncludeNode (fileName);
        }
        else
        {
            error_handle::raise(error_handle_type::PARSER, "Expected file name in <> or in \"\"");
            return nullptr;
        }
    }
    // READY
    ASTNode* expressionParse() {
        ASTNode* leftOperand = primaryExpressionParse();

        while (is_binary_operator(get_token_type())) {
            token_type binaryOperator = get_token_type();
            if (binaryOperator == token_type::ASSIGN) {
                // Обрабатываем оператор присваивания
                ASTNode* assignment = assignmentStatementParse();
                leftOperand = assignment;
            } else {
                next_token();
                ASTNode* rightOperand = primaryExpressionParse();
                leftOperand = new BinaryOperationNode(leftOperand, binaryOperator, rightOperand);
            }
            
        }
        return leftOperand;


    }
    // READY
    ASTNode* primaryExpressionParse() {
        token_type currentTokenType = get_token_type();

        if (currentTokenType == token_type::IDENTIFIER) {
            std::string identifier = get_token_lexeme();
            next_token(); // Переход на ; или ,
            return new IdentifierNode(identifier);
        }

        if (currentTokenType == token_type::INTEGER_CONST || currentTokenType == token_type::DOUBLE_CONST) {
            std::string value = get_token_lexeme();
            next_token(); // Переход на операнд;
            return new LiteralNode(value);
        }
        error_handle::raise(error_handle_type::PARSER, "An identifire or const is exected!!");
        return nullptr;
    }
    // READY
    ASTNode* variableDeclarationParse() {

        next_token();
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
            error_handle::raise(error_handle_type::PARSER, "The left bracket is expected!!!");
            return nullptr;
        }

        next_token(); //Скобка или тип

        if (get_token_type() == token_type::RPAR) {
            return nullptr;
        }

        statements.push_back(variableDeclarationParse());
        if (get_token_type() != token_type::RPAR && get_token_type() != token_type::COMMA) {
            error_handle::raise(error_handle_type::PARSER, "Check the arguments of the function!!!");
            return nullptr;
        }

        while (get_token_type() == token_type::COMMA) {
            next_token();
            statements.push_back(variableDeclarationParse());
        }

        if (get_token_type() != token_type::RPAR) {
            error_handle::raise(error_handle_type::PARSER, "The right bracket is expected!!!");
            return nullptr;
        }

        return new ParameterListNode(statements);
    }

    ASTNode* compoundStatementsParse() {
        

        if (get_token_type() != token_type::LBRA) {
            error_handle::raise(error_handle_type::PARSER, "An left brace is expected");
            return nullptr;
        }

        next_token();
        std::vector<ASTNode*> statements;
        while (get_token_type() != token_type::RBRA) {
            ASTNode* statement = parseStatement();
            if (statement != nullptr) {
                statements.push_back(statement);
                if (currentTokenIndex + 1 < tokens.size()) {
                    next_token();
                }
                    
            }
        }

        if (get_token_type() != token_type::RBRA) {
            error_handle::raise(error_handle_type::PARSER, "Expected closing curly brace for compound statement.");
            return nullptr;
        }

        if (currentTokenIndex + 1 < tokens.size()) {
            next_token();
        }

        return new CompoundStatementNode(statements);
    }

    ASTNode* compaundParse() {
        token_type currentTokenType = get_token_type();

        if (currentTokenType == token_type::INT || currentTokenType == token_type::DOUBLE) {
            return variableDeclarationParse();
        }
        else if (currentTokenType == token_type::IF || currentTokenType == token_type::ELSE) {
            return parseIfElseExpression();
        }
        else {
            // Все остальные случаи считаем выражением
            return expressionParse();
        }
    }

    ASTNode* parseStatement() {
        token_type currentTokenType = get_token_type();

        if (currentTokenType == token_type::INT || currentTokenType == token_type::DOUBLE || currentTokenType == token_type::VOID) {
            // Объявление переменной
            return variableDeclarationParse();
        }
        else if (currentTokenType == token_type::IF || currentTokenType == token_type::ELSE) {
            // Оператор if-else
            return parseIfElseExpression();
        }
        else {
            // Все остальные случаи считаем выражением
            return expressionParse();
        }
    }

    ASTNode* functionDeclarationParse() {
        token_type returnType = get_token_type(); // Тип
        next_token(); 
        std::string name = get_token_lexeme(); // Идентификатор
        next_token(); // Скобка
        ASTNode* parameters = functionParametersParse(); 
        next_token(); // Фигурная скобка
        ASTNode* compounds = compoundStatementsParse(); // Парсинг блока функции
        next_token();

        return new FunctionDeclarationNode(parameters, compounds, name, returnType);
        
    }
    ASTNode* parseIfElseExpression() {
        next_token(); // Пропускаем "if" или "else"

        if (get_token_type() != token_type::LPAR) {
            error_handle::raise(error_handle_type::PARSER, "Expected opening parenthesis after if/else.");
            return nullptr;
        }

        next_token();
        ASTNode* condition = expressionParse();

        if (get_token_type() != token_type::RPAR) {
            error_handle::raise(error_handle_type::PARSER, "Expected closing parenthesis after if/else condition.");
            return nullptr;
        }

        next_token();
        ASTNode* codeBlock = compoundStatementsParse();

        // Опциональный блок else
        if (get_token_type() == token_type::ELSE) {
            next_token();
            ASTNode* elseBlock = compoundStatementsParse();
            return new ElseIFNode(condition, codeBlock, elseBlock);
        }

        return new IfNode(condition, codeBlock);
    }

    ASTNode* assignmentStatementParse() {
        // Парсим идентификатор
        std::string variableIdentifier = get_token_lexeme();
        if (get_token_type() == token_type::ASSIGN) {
            next_token();

            // Парсим выражение
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

};
