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

    ASTNode* parseStatements() {
        if (get_token_type() == token_type::DIRECTIVE_INCLUDE) {
            return preprocessorDirectiveParse();
        }
        else if (get_token_type() == token_type::INT || get_token_type() == token_type::DOUBLE || get_token_type() == token_type::BOOL || get_token_type() == token_type::CHAR || get_token_type() == token_type::STRING || get_token_type() == token_type::VOID || get_token_type() == token_type::AUTO) {
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

    ASTNode* preprocessorDirectiveParse() {
        next_token();
        if (get_token_type() == token_type::LESS && get_token_type_offset(2) == token_type::GREATER) {
            next_token();
            return new DerectivIncludeNode (get_token_lexeme());
        }
        else if (get_token_type() == token_type::STRING_CONST) {
            next_token();
            std::string fileName = get_token_lexeme().substr(1, fileName.size() - 2);
            return new DerectivIncludeNode (fileName);
        }
        else
        {
            error_handle::raise(error_handle_type::PARSER, "Expected file name in <> or in \"\"");
            return nullptr;
        }
    }

    ASTNode* variableDeclarationParse() {

    }

    ASTNode* functionDeclarationParse() {
        std::string returnType = get_token_lexeme();
        next_token();
        std::string name = get_token_lexeme();
        while (get_token_type() != token_type::LBRA) {
            next_token();
        }
        next_token();

    }

    bool bracketsBalance(const token token) {
        std::stack<token_type> brackets;
        if (token.type == token_type::LPAR || token.type == token_type::LSQR || token.type == token_type::LBRA) {
            brackets.push(token.type);
        }
        else if (token.type == token_type::RPAR || token.type == token_type::RSQR || token.type == token_type::RBRA) {
            if (brackets.empty()) {
                return false;
            }

            token_type top = brackets.top();
            brackets.pop();

            if ((token.type == token_type::RPAR && top != token_type::LPAR) ||
                (token.type == token_type::RSQR && top != token_type::LSQR) ||
                (token.type == token_type::RBRA && top != token_type::LBRA)) {
                return false;
            }
        }
        return brackets.empty();
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

};
