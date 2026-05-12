#include <iostream>
#include <sstream>
#include "MyString.h"
#include "MyVector.h"

// Функция для разбиения строки на вектор слов
MyStringVector splitCommand(const MyString& input) {
	MyStringVector tokens;
	std::istringstream stream(input);
	MyString token;

	while (stream >> token) {
		tokens.push_back(token);
	}
	return tokens;
}

void executeCommand(MyString& command) {
	MyStringVector args = splitCommand(command);
	std::cout << args[0] << "\n";
}

int main()
{
	MyString input;

	while (true)
	{
		std::getline(std::cin, input);

		if (input == "quit") break;
		if (input.empty()) continue;

		std::cout << input << "\n";

		executeCommand(input);

	}
}


