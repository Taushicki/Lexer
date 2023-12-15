#include <iostream>
#include "parser.h"
#include "lexer.h"



int main() {
	
	try {
		lexer lex("test2.txt");
		lex.print();
		parser pars(lex.tokens);
		ASTNode * root = pars.parse();
		root->print();

		delete root;
;
	}
	catch (const exception& e) {
		std::cout << "Exception is called\n" << e.what() << endl;;
	}

	

	return 0;
}