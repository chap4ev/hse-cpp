#include <iostream>
#include <string>

#include "biginteger.h"

int main() {
	BigInteger a, b;
	std::string op;
	
	std::cout << "Expression format \"<number> <operation> <number>\"\n";
	std::cout << "Supported operations: + - * / % \n";
	std::cout << "Input expression: ";

	std::cin >> a >> op >> b;

	std::cout << a << " " << op << " " << b << " = ";

	if (op == "+") {
		std::cout << a + b;

	} else if (op == "-") {
		std::cout << a - b;

	} else if (op == "*") {
		std::cout << a * b;

	} else if (op == "/") {
		std::cout << (a /= b);

	} else if (op == "%") {
		std::cout << a % b;
	}

	std::cout << std::endl;

	return 0;
}