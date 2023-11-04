#include <iostream>

#include "lexer.h"

int main() {
	
	try {
		lexer lex("test.txt");
		lex.print();
	}
	catch (const exception& e) {
		std::cout << "Exception is called\n" << e.what() << endl;;
	}
	
	return 0;
}