//#include <iostream>
////#include "parser.h"
////#include "node.h"
//
//// �������, ������� ������ ��������� ������ if-else
//class parser{
//public:
//    parser(const std::vector<token>& tokens) : tokens(tokens), currentTokenIndex(0) {} // ������� ������ � ��������
//
//private:
//    const std::vector<token>& tokens;
//    size_t currentTokenIndex;
//    ASTNode* parseIfElseExpression() {
//        next_token(); // ���������� "if" ��� "else"
//
//        // ������� ����������� ������
//        if (get_token_type() != token_type::LPAR) {
//            error_handle::raise(error_handle_type::PARSER, "Expected opening parenthesis after if/else.");
//            return nullptr;
//        }
//
//        // ������ �������
//        next_token();
//        ASTNode* condition = expressionParse();
//
//        // ������� ����������� ������
//        if (get_token_type() != token_type::RPAR) {
//            error_handle::raise(error_handle_type::PARSER, "Expected closing parenthesis after if/else condition.");
//            return nullptr;
//        }
//
//        // ������ ���� ����
//        next_token();
//        ASTNode* codeBlock = compoundStatementParse();
//
//        // ������������ ���� else
//        if (get_token_type() == token_type::ELSE) {
//            next_token();
//            ASTNode* elseBlock = compoundStatementParse();
//            return new IfElseNode(condition, codeBlock, elseBlock);
//        }
//
//        return new IfNode(condition, codeBlock);
//    }
//
//    // �������, ������� ������ ���������� ����������
//    ASTNode* parseVariableDeclaration() {
//        next_token(); // ���������� ��� ����������
//
//        // ������� ������������� ����������
//        if (get_token_type() != token_type::IDENTIFIER) {
//            error_handle::raise(error_handle_type::PARSER, "Expected variable identifier after type.");
//            return nullptr;
//        }
//
//        std::string variableIdentifier = get_token_lexeme();
//
//        next_token(); // ���������� �������������
//
//        // ������������ ������������ �������� ����������
//        if (get_token_type() == token_type::ASSIGN) {
//            next_token();
//            ASTNode* expression = expressionParse();
//            return new VariableDeclarationNode(variableIdentifier, expression);
//        }
//
//        return new VariableDeclarationNode(variableIdentifier, nullptr);
//    }
//
//    // �������, ������� ������ �������� (���������, ���������� ����������, if-else)
//    ASTNode* parseStatement() {
//        token_type currentTokenType = get_token_type();
//
//        if (currentTokenType == token_type::INT || currentTokenType == token_type::DOUBLE || currentTokenType == token_type::VOID) {
//            // ���������� ����������
//            return parseVariableDeclaration();
//        }
//        else if (currentTokenType == token_type::IF || currentTokenType == token_type::ELSE) {
//            // �������� if-else
//            return parseIfElseExpression();
//        }
//        else {
//            // ��� ��������� ������ ������� ����������
//            return expressionParse();
//        }
//    }
//
//    // �������, ������� ������ ���� ���� (��������, ���� �������)
//    ASTNode* compoundStatementParse() {
//        // ������� ����������� �������� ������
//        if (get_token_type() != token_type::LBRACE) {
//            error_handle::raise(error_handle_type::PARSER, "Expected opening curly brace for compound statement.");
//            return nullptr;
//        }
//
//        next_token();
//
//        // ������ ��������� ������ �����
//        std::vector<ASTNode*> statements;
//        while (get_token_type() != token_type::RBRACE) {
//            ASTNode* statement = parseStatement();
//            if (statement != nullptr) {
//                statements.push_back(statement);
//            }
//        }
//
//        // ������� ����������� �������� ������
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
//    // �������, ������� ������ �������
//    ASTNode* parseFunction() {
//        next_token(); // ���������� ��� ������� (��������, int)
//
//        // ������� ������������� �������
//        if (get_token_type() != token_type::IDENTIFIER) {
//            error_handle::raise(error_handle_type::PARSER, "Expected function identifier after type.");
//            return nullptr;
//        }
//
//        std::string functionName = get_token_lexeme();
//
//        next_token(); // ���������� �������������
//
//        // ������� ����������� ������
//        if (get_token_type() != token_type::LPAREN) {
//            error_handle::raise(error_handle_type::PARSER, "Expected opening parenthesis after function identifier.");
//            return nullptr;
//        }
//
//        // ������ ��������� �������
//        next_token();
//        std::vector<ASTNode*> parameters;
//        while (get_token_type() != token_type::RPAREN) {
//            ASTNode* parameter = parseVariableDeclaration();
//            if (parameter != nullptr) {
//                parameters.push_back(parameter);
//            }
//
//            // ������� ������� ����� �����������
//            if (get_token_type() == token_type::COMMA) {
//                next_token();
//            }
//        }
//
//        // ������� ����������� ������
//        if (get_token_type() != token_type::RPAREN) {
//            error_handle::raise(error_handle_type::PARSER, "Expected closing parenthesis after function parameters.");
//            return nullptr;
//        }
//
//        next_token(); // ���������� ����������� ������
//
//        // ������ ���� ������� (���� ����)
//        ASTNode* functionBody = compoundStatementParse();
//
//        return new FunctionDeclarationNode(functionName, parameters, functionBody);
//    }
//
//    // �������� ������� �������� ���������
//    ASTNode* parseProgram() {
//        std::vector<ASTNode*> statements;
//
//        // ������ ��������� � ���������� ������� ���������
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