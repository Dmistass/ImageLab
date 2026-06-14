#pragma once
#include "MyString.h"
#include "MyVector.h"

// Base class for all commands
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;

    static Command* create(const MyString& name, const MyVector<MyString>& args);
};

// Loads one or more images (load <path1> <path2> ...)
class LoadImagesCommand : public Command {
    MyVector<MyString> paths;  // list of file paths
public:
    LoadImagesCommand(const MyVector<MyString>& args);
    void execute() override;
};

// Removes an image by name (remove <name>)
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

// Adds a filter to an image (add-filter <image> <filter>)
class AddFilterCommand : public Command {
    MyString imageName;  // image name
    MyString filterName; // filter name
public:
    AddFilterCommand(const MyVector<MyString>& args);
    void execute() override;
};

// Removes a filter from the list by index (remove-filter <image> <index>)
class RemoveFilterCommand : public Command {
    MyString imageName;
    int filterIndex = 0;  // filter index (from show-filters)
public:
    RemoveFilterCommand(const MyVector<MyString>& args);
    void execute() override;
};

// Applies all filters to one image (run <image>)
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

// Saves an image (save <image> [path]).
class SaveImageCommand : public Command {
    MyString name;       // image name
    MyString savePath;   // path for saving (may be empty)
public:
    SaveImageCommand(const MyVector<MyString>& args);
    void execute() override;
};

class HelpCommand : public Command {
public:
    void execute() override;
};

class ShowAvailableFiltersCommand : public Command {
public:
    void execute() override;
};