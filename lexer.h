#pragma once
#include <iostream>
#include "token.h"
#include <vector>
#include <fstream>
#include "error_handle.h"

class lexer {

private:
	
	string file_path;		// Путь к файлу
	string code;			// Строка с кодом из файла

	vector<token> tokens;	// Вектор с токенами

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
	// Метод проверки на состовные оператор
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
	//Метод проверки на блок комментария
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
	// Метод проверки на разделительный символ
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
	// Метод проверки на char
	// Метод проверки на string

	/*static bool is_comment_string(char symbol1, char symbol2) {
		if (symbol1 == '/') {
			return symbol2 == '/';
		}
		else {
			false;
		}
	}*/

	// Открытие файла
	void open() {
		ifstream in(file_path, ios::binary);

		// Ошибка на этапе лексического анализа. Файл не найден 
		if (!in.is_open()) {
			error_handle::raise(error_handle_type::LEXER, "File not found!");
		}

		// Узнаём размер файл и переводим указатель в начало 
		size_t size = in.seekg(0, ios::end).tellg();
		in.seekg(0);

		// Считываем файл в переменную 'code'
		code.resize(size);
		in.read(&code[0], size);
		in.close();
	}
	// Метод разбития строки 'code'
	void split() {
		
		string temp_lexeme;		// Временная переменная для хранения лексемы

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

	// Перепроверяем токены на наличие состовных операторов, чисел с плавающей точкой, комментариев, char'ов, строк
	void merge() {

		vector<token> temp_tokens;

		for (int index = 0; index < tokens.size(); index++) {

			// Число с плавающей точкой
			if (tokens[index].type == token_type::INTEGER_CONST) {

				if (index + 1 < tokens.size() && index + 2 < tokens.size() &&  tokens[index + 1].type == token_type::POINT && tokens[index + 2].type == token_type::INTEGER_CONST) {

					string new_lexeme = tokens[index].lexeme + tokens[index + 1].lexeme + tokens[index + 2].lexeme;
					token double_token(new_lexeme);
					temp_tokens.push_back(double_token);

					index += 2;
					continue;
				}
			}

			// Составной оператор
			if (index + 1 < tokens.size() && is_complex_operator(tokens[index].lexeme[0], tokens[index + 1].lexeme[0])) {

				string new_lexeme = tokens[index].lexeme + tokens[index + 1].lexeme;
				token complex_token(new_lexeme);
				temp_tokens.push_back(complex_token);

				index += 1;
				continue;
			}

			// Комментарии
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

			// Строка
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