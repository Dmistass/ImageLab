#pragma once
#include "MyString.h"
#include "MyVector.h"

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;

    // Статическая фабрика — создаёт команду по имени
    // args — чистые аргументы (без имени команды)
    static Command* create(const MyString& name, const MyVector<MyString>& args);
};

// ===== Наследники =====

class LoadImagesCommand : public Command {
    MyVector<MyString> paths;
public:
    LoadImagesCommand(const MyVector<MyString>& args);
    void execute() override;
};

class RemoveImageCommand : public Command {
    MyString name;
public:
    RemoveImageCommand(const MyVector<MyString>& args);
    void execute() override;
};

class PrintAllCommand : public Command {
public:
    void execute() override;
};

class AddFilterCommand : public Command {
    MyString imageName;
    MyString filterName;
public:
    AddFilterCommand(const MyVector<MyString>& args);
    void execute() override;
};

class RemoveFilterCommand : public Command {
    MyString imageName;
    int filterIndex = 0;
public:
    RemoveFilterCommand(const MyVector<MyString>& args);
    void execute() override;
};

class RunCommand : public Command {
    MyString imageName;
public:
    RunCommand(const MyVector<MyString>& args);
    void execute() override;
};

class RunAllCommand : public Command {
public:
    void execute() override;
};

class ShowFiltersCommand : public Command {
    MyString name;
public:
    ShowFiltersCommand(const MyVector<MyString>& args);
    void execute() override;
};

class ShowAllFiltersCommand : public Command {
public:
    void execute() override;
};

class SaveImageCommand : public Command {
    MyString name;
    MyString savePath;
public:
    SaveImageCommand(const MyVector<MyString>& args);
    void execute() override;
};