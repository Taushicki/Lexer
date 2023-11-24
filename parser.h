#pragma once
#include <iostream>
#include <string>
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
        if (get_token_type() == token_type::PREPROCESSOR_DIRECTIVE) {
            preprocessorDirectiveParse();
        }
        else if(get_token_type() == token_type::INT ||
            get_token_type() == token_type::DOUBLE ||
            get_token_type() == token_type::BOOL ||
            get_token_type() == token_type::CHAR ||
            get_token_type() == token_type::STRING||
            get_token_type() == token_type::VOID ||
            get_token_type() == token_type::AUTO) 
        {
            if (get_token_type_offset(2) == token_type::LPAR) {
                functionDeclarationParse();
            } else if (get_token_type_offset(2) == token_type::ASSIGN || get_token_type_offset(2) == token_type::SEMICOLON)
                variableDeclarationParse();
            }       
            else {
                error_handle::raise(error_handle_type::PARSER, "Must be inside the function!");
            }
        
    }

    ASTNode* preprocessorDirectiveParse() {

    }

    ASTNode* variableDeclarationParse() {

    }

    ASTNode* functionDeclarationParse() {

    }

    token_type get_token_type() {
        return tokens[currentTokenIndex].type;
    }

    token_type get_token_type_offset(int offset) {
        return tokens[currentTokenIndex + offset].type;
    }

    void next_token() {
        currentTokenIndex++;
    }

};
