#pragma once
#include <iostream>
#include "token.h"
#include <vector>
#include <fstream>
#include "error_handle.h"

class lexer {

private:
	
	string file_path;		// ���� � �����
	string code;			// ������ � ����� �� �����

	vector<token> tokens;	// ������ � ��������

public:

	explicit lexer(const string& file_path) {
		this->file_path = file_path;
		this->open();
		this->split();
	}

	void print() {

		for (auto& token : tokens)
		{
			token.print();
		}
	}

	

private:
	// ����� �������� �� ��������� ��������
	static bool is_complex_operator(char symbol1, char symbol2) {
		switch (symbol1)
		{
		case '<':
		{
			return symbol2 == '=';
		}
		case '>':
		{
			return symbol2 == '=';
		}
		case '+':
		{
			return symbol2 == '=' || symbol2 == '+';
		}
		case '-':
		{
			return symbol2 == '=' || symbol2 == '-';
		}
		case '=':
		{
			return symbol2 == '=';
		}
		case '!':
		{
			return symbol2 == '=';
		}
		case '&':
		{
			return symbol2 == '&';
		}
		case '|':
		{
			return symbol2 == '|';
		}
		case ':':
		{
			return symbol2 == ':';
		}
		default:
			return false;
		}
	}
	//����� �������� �� ���� �����������
	static bool is_comment_block(char symbol1, char symbol2) {
		switch (symbol1) {
		case '/':
		{
			return symbol2 == '*';
		}
		case '*':
		{
			return symbol2 == '/';
		}
		default:
			return false;
		}
	}
	// ����� �������� �� �������������� ������
	static bool is_separete_symbol(char symbol) {
		return  symbol == ':' || symbol == ';' ||
			symbol == ',' || symbol == '.' ||
			symbol == '{' || symbol == '}' ||
			symbol == '(' || symbol == ')' ||
			symbol == '[' || symbol == ']' ||
			symbol == '*' || symbol == '/' ||
			symbol == '+' || symbol == '-' ||
			symbol == '&' || symbol == '|' ||
			symbol == '=' || symbol == '!' ||
			symbol == '<' || symbol == '>' ||
			symbol == '\'' || symbol == '"' ||
			symbol == '^' || symbol == '?' ||
			symbol == '%' || symbol == '\\' ||
			symbol == '~' || symbol == ' ' ||
			symbol == '\r' || symbol == '\n' ||
			symbol == '#' || symbol == '\t';
	}
	// ����� �������� �� char
	// ����� �������� �� string

	/*static bool is_comment_string(char symbol1, char symbol2) {
		if (symbol1 == '/') {
			return symbol2 == '/';
		}
		else {
			false;
		}
	}*/

	// �������� �����
	void open() {
		ifstream in(file_path, ios::binary);

		// ������ �� ����� ������������ �������. ���� �� ������ 
		if (!in.is_open()) {
			error_handle::raise(error_handle_type::LEXER, "File not found!");
		}

		// ����� ������ ���� � ��������� ��������� � ������ 
		size_t size = in.seekg(0, ios::end).tellg();
		in.seekg(0);

		// ��������� ���� � ���������� 'code'
		code.resize(size);
		in.read(&code[0], size);
		in.close();
	}
	// ����� �������� ������ 'code'
	void split() {
		
		string temp_lexeme;		// ��������� ���������� ��� �������� �������

		for (const auto& symbol : code) {

			if (is_separete_symbol(symbol)) {

				if (!temp_lexeme.empty()) {

					token new_token(temp_lexeme);
					tokens.push_back(new_token);
					temp_lexeme.clear();
				}

				if (symbol != ' ' && symbol != '\n' && symbol != '\r' && symbol != '\t') {
					string current_symbol(1, symbol);
					token new_token(current_symbol);
					tokens.push_back(new_token);
				}
			}
			else {
				temp_lexeme += symbol;
			}
		}
		merge();
	}

	// ������������� ������ �� ������� ��������� ����������, ����� � ��������� ������, ������������, char'��, �����
	void merge() {

		vector<token> temp_tokens;

		for (int index = 0; index < tokens.size(); index++) {

			// ����� � ��������� ������
			if (tokens[index].type == token_type::INTEGER_CONST) {

				if (index + 1 < tokens.size() && index + 2 < tokens.size() &&  tokens[index + 1].type == token_type::POINT && tokens[index + 2].type == token_type::INTEGER_CONST) {

					string new_lexeme = tokens[index].lexeme + tokens[index + 1].lexeme + tokens[index + 2].lexeme;
					token double_token(new_lexeme);
					temp_tokens.push_back(double_token);

					index += 2;
					continue;
				}
			}

			// ��������� ��������
			if (index + 1 < tokens.size() && is_complex_operator(tokens[index].lexeme[0], tokens[index + 1].lexeme[0])) {

				string new_lexeme = tokens[index].lexeme + tokens[index + 1].lexeme;
				token complex_token(new_lexeme);
				temp_tokens.push_back(complex_token);

				index += 1;
				continue;
			}

			// �����������
			if (index + 1 < tokens.size() && is_comment_block(tokens[index].lexeme[0], tokens[index + 1].lexeme[0])) {

				string new_lexeme = tokens[index].lexeme + tokens[index + 1].lexeme;
				token start_coment_token(new_lexeme);
				temp_tokens.push_back(start_coment_token);

				index += 1;

				while (!is_comment_block(tokens[index].lexeme[0], tokens[index + 1].lexeme[0])) {
					index++;
				}

				new_lexeme = tokens[index].lexeme + tokens[index + 1].lexeme;
				token end_coment_token(new_lexeme);
				temp_tokens.push_back(end_coment_token);

				index += 1;
				continue;
			}
			
			// Char
			if (tokens[index].type == token_type::CHAR_CONST) {

				string new_lexeme = tokens[index].lexeme;

				index++;

				while (tokens[index].type != token_type::CHAR_CONST)
				{
					new_lexeme += tokens[index].lexeme;
					index++;
				}

				new_lexeme += tokens[index].lexeme;
				token char_const(new_lexeme);
				temp_tokens.push_back(char_const);

				continue;

			}

			// ������
			if (tokens[index].type == token_type::STRING_CONST) {

				string new_lexeme = tokens[index].lexeme;

				index++;

				while (tokens[index].type != token_type::STRING_CONST)
				{
					new_lexeme += tokens[index].lexeme;
					index++;
				}

				new_lexeme += tokens[index].lexeme;
				token char_const(new_lexeme);
				temp_tokens.push_back(char_const);

				continue;

			}

			/*if (index + 1 < tokens.size() && is_comment_string(tokens[index].lexeme[0], tokens[index + 1].lexeme[0])) {
				while (tokens[index].lexeme[0] != '\n' || tokens[index].lexeme[0] != '\r') {
					index++;
				}
				continue;
			}*/
			temp_tokens.push_back(tokens[index]);
		}

		tokens = temp_tokens;
		temp_tokens.clear();
	}

	
};