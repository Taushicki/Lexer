//#include <iostream>
////#include "parser.h"
////#include "node.h"
//
//// Функция, которая парсит выражение внутри if-else
//class parser{
//public:
//    parser(const std::vector<token>& tokens) : tokens(tokens), currentTokenIndex(0) {} // Передаём вектор с токенами
//
//private:
//    const std::vector<token>& tokens;
//    size_t currentTokenIndex;
//    ASTNode* parseIfElseExpression() {
//        next_token(); // Пропускаем "if" или "else"
//
//        // Ожидаем открывающую скобку
//        if (get_token_type() != token_type::LPAR) {
//            error_handle::raise(error_handle_type::PARSER, "Expected opening parenthesis after if/else.");
//            return nullptr;
//        }
//
//        // Парсим условие
//        next_token();
//        ASTNode* condition = expressionParse();
//
//        // Ожидаем закрывающую скобку
//        if (get_token_type() != token_type::RPAR) {
//            error_handle::raise(error_handle_type::PARSER, "Expected closing parenthesis after if/else condition.");
//            return nullptr;
//        }
//
//        // Парсим блок кода
//        next_token();
//        ASTNode* codeBlock = compoundStatementParse();
//
//        // Опциональный блок else
//        if (get_token_type() == token_type::ELSE) {
//            next_token();
//            ASTNode* elseBlock = compoundStatementParse();
//            return new IfElseNode(condition, codeBlock, elseBlock);
//        }
//
//        return new IfNode(condition, codeBlock);
//    }
//
//    // Функция, которая парсит объявление переменной
//    ASTNode* parseVariableDeclaration() {
//        next_token(); // Пропускаем тип переменной
//
//        // Ожидаем идентификатор переменной
//        if (get_token_type() != token_type::IDENTIFIER) {
//            error_handle::raise(error_handle_type::PARSER, "Expected variable identifier after type.");
//            return nullptr;
//        }
//
//        std::string variableIdentifier = get_token_lexeme();
//
//        next_token(); // Пропускаем идентификатор
//
//        // Опциональное присваивание значения переменной
//        if (get_token_type() == token_type::ASSIGN) {
//            next_token();
//            ASTNode* expression = expressionParse();
//            return new VariableDeclarationNode(variableIdentifier, expression);
//        }
//
//        return new VariableDeclarationNode(variableIdentifier, nullptr);
//    }
//
//    // Функция, которая парсит оператор (выражение, объявление переменной, if-else)
//    ASTNode* parseStatement() {
//        token_type currentTokenType = get_token_type();
//
//        if (currentTokenType == token_type::INT || currentTokenType == token_type::DOUBLE || currentTokenType == token_type::VOID) {
//            // Объявление переменной
//            return parseVariableDeclaration();
//        }
//        else if (currentTokenType == token_type::IF || currentTokenType == token_type::ELSE) {
//            // Оператор if-else
//            return parseIfElseExpression();
//        }
//        else {
//            // Все остальные случаи считаем выражением
//            return expressionParse();
//        }
//    }
//
//    // Функция, которая парсит блок кода (например, тело функции)
//    ASTNode* compoundStatementParse() {
//        // Ожидаем открывающую фигурную скобку
//        if (get_token_type() != token_type::LBRACE) {
//            error_handle::raise(error_handle_type::PARSER, "Expected opening curly brace for compound statement.");
//            return nullptr;
//        }
//
//        next_token();
//
//        // Парсим операторы внутри блока
//        std::vector<ASTNode*> statements;
//        while (get_token_type() != token_type::RBRACE) {
//            ASTNode* statement = parseStatement();
//            if (statement != nullptr) {
//                statements.push_back(statement);
//            }
//        }
//
//        // Ожидаем закрывающую фигурную скобку
//        if (get_token_type() != token_type::RBRACE) {
//            error_handle::raise(error_handle_type::PARSER, "Expected closing curly brace for compound statement.");
//            return nullptr;
//        }
//
//        next_token();
//
//        return new CompoundStatementNode(statements);
//    }
//
//    // Функция, которая парсит функцию
//    ASTNode* parseFunction() {
//        next_token(); // Пропускаем тип функции (например, int)
//
//        // Ожидаем идентификатор функции
//        if (get_token_type() != token_type::IDENTIFIER) {
//            error_handle::raise(error_handle_type::PARSER, "Expected function identifier after type.");
//            return nullptr;
//        }
//
//        std::string functionName = get_token_lexeme();
//
//        next_token(); // Пропускаем идентификатор
//
//        // Ожидаем открывающую скобку
//        if (get_token_type() != token_type::LPAREN) {
//            error_handle::raise(error_handle_type::PARSER, "Expected opening parenthesis after function identifier.");
//            return nullptr;
//        }
//
//        // Парсим параметры функции
//        next_token();
//        std::vector<ASTNode*> parameters;
//        while (get_token_type() != token_type::RPAREN) {
//            ASTNode* parameter = parseVariableDeclaration();
//            if (parameter != nullptr) {
//                parameters.push_back(parameter);
//            }
//
//            // Ожидаем запятую между параметрами
//            if (get_token_type() == token_type::COMMA) {
//                next_token();
//            }
//        }
//
//        // Ожидаем закрывающую скобку
//        if (get_token_type() != token_type::RPAREN) {
//            error_handle::raise(error_handle_type::PARSER, "Expected closing parenthesis after function parameters.");
//            return nullptr;
//        }
//
//        next_token(); // Пропускаем закрывающую скобку
//
//        // Парсим тело функции (блок кода)
//        ASTNode* functionBody = compoundStatementParse();
//
//        return new FunctionDeclarationNode(functionName, parameters, functionBody);
//    }
//
//    // Основная функция парсинга программы
//    ASTNode* parseProgram() {
//        std::vector<ASTNode*> statements;
//
//        // Парсим операторы в глобальной области видимости
//        while (get_token_type() != token_type::END_OF_FILE) {
//            ASTNode* statement = parseStatement();
//            if (statement != nullptr) {
//                statements.push_back(statement);
//            }
//        }
//
//        return new ProgramNode(statements);
//    }
//    token_type get_token_type() {
//        return tokens[currentTokenIndex].type;
//    }
//
//    std::string get_token_lexeme() {
//        return tokens[currentTokenIndex].lexeme;
//
//    }
//
//    void next_token() {
//        currentTokenIndex++;
//    }
//};