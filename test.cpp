#include <string>

using namespace std;

int print() {

	/*������ ��� ������� ����*/

	int new_int = 1;
	new_int += 1;
	double new_double = 2.0;
	new_double -= 1;
	bool new_boolean = true;
	new_boolean = false;
	char new_char = '"';
	string new_string = "Hello my \n dear friend";
	auto new_auto = 10;

	if (new_int <= 100) {
		return 0;
	}
	else if (new_int == 10) {
		return 0;
	}
	else {
		return 1;
	}


	
	return 0;
}
