#include <iostream>

#include "lexer.h"

int main() {
	lexer lex("test.cpp");
	lex.print();
	
	return 0;
}