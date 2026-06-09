#include <iostream>
#include <sstream>
#include "MyString.h"
#include "MyVector.h"
#include "Commands.h"

// Функция для разбиения строки на вектор слов
MyVector<MyString> splitCommand(const MyString& input) {
	MyVector<MyString> tokens;
	std::istringstream stream(input);
	MyString token;

	while (stream >> token) {
		tokens.push_back(token);
	}
	return tokens;
}

void executeCommand(MyString& command) {
	MyVector<MyString> args = splitCommand(command);
	int argsCount = args.size();

	MyString cmdName = args[0];

	if (cmdName == "load" && argsCount >= 2) {
		MyVector<MyString> paths;
		for (int i = 1; i < argsCount; i++)
		{
			paths.push_back(args[i]);
		}
		loadImages(paths);
	}
	else if (cmdName == "remove" && argsCount == 2) {
		removeImage(args[1]);
	}
	else if (cmdName == "printAll"){
		printAll();
	}
	else if (cmdName == "add-filter" && argsCount == 3) {
		addFilter(args[1], args[2]);
	}
	else if (cmdName == "remove-filter" && argsCount == 3) {
		removeFilter(args[1], std::stoi(args[2]));
	}
	else if (cmdName == "show-filters" && argsCount == 2) {
		showFilters(args[1]);
	}
	else if (cmdName == "show-all-filters") {
		showAllFilters();
	}
	else if (cmdName == "save" && argsCount == 2) {
		MyString emptyPath;
		saveImage(args[1], emptyPath);
	}
	else if (cmdName == "save" && argsCount == 3) {
		saveImage(args[1], args[2]);
	}
	else if (cmdName == "run" && argsCount == 2) {
		run(args[1]);
	}
	else if (cmdName == "run-all") {
		runAll();
	}
	else {
		std::cout << "Invalid command!\n";
	}
}

int main()
{
	MyString input;

	while (true)
	{
		std::cout << ">> ";
		std::getline(std::cin, input);

		if (input == "quit") break;
		if (input.empty()) continue;

		executeCommand(input);
	}
}


