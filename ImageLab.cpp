#include <iostream>
#include <sstream>
#include "MyString.h"
#include "MyVector.h"
#include "Commands.h"

// Функция для разбиения строки на вектор слов
MyVector<MyString> splitCommand(const MyString& input) {
	MyVector<MyString> tokens;
	std::istringstream stream(input.c_str());
	MyString token;

	while (stream >> token) {
		tokens.push_back(token);
	}
	return tokens;
}

void executeCommand(MyString& command) {
	MyVector<MyString> args = splitCommand(command);
	if (args.size() == 0) return;

	MyString cmdName = args[0];

	// Удаляем первый элемент (имя команды) — остаются только чистые аргументы
	MyVector<MyString> cmdArgs;
	for (size_t i = 1; i < args.size(); i++)
		cmdArgs.push_back(args[i]);

	Command* cmd = Command::create(cmdName, cmdArgs);
	if (!cmd) {
		std::cout << "Invalid command!\n";
		return;
	}

	cmd->execute();
	delete cmd;
}

int main()
{
	MyString input;

	while (true)
	{
		std::cout << ">> ";
		getline(std::cin, input);

		if (input == "quit") break;
		if (input.empty()) continue;

		executeCommand(input);
	}
}