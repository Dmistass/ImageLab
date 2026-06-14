#include <iostream>
#include <cstdlib>
#include "Commands.h"
#include "ImageList.h"

// ===== Статическая фабрика Command::create =====
Command* Command::create(const MyString& name, const MyVector<MyString>& args) {
    if (name == "load" && args.size() >= 1)
        return new LoadImagesCommand(args);
    if (name == "remove" && args.size() == 1)
        return new RemoveImageCommand(args);
    if (name == "printAll" && args.size() == 0)
        return new PrintAllCommand();
    if (name == "add-filter" && args.size() == 2)
        return new AddFilterCommand(args);
    if (name == "remove-filter" && args.size() == 2)
        return new RemoveFilterCommand(args);
    if (name == "show-filters" && args.size() == 1)
        return new ShowFiltersCommand(args);
    if (name == "show-all-filters" && args.size() == 0)
        return new ShowAllFiltersCommand();
    if (name == "save" && args.size() >= 1)
        return new SaveImageCommand(args);
    if (name == "run" && args.size() == 1)
        return new RunCommand(args);
    if (name == "run-all" && args.size() == 0)
        return new RunAllCommand();
    if (name == "help" && args.size() == 0)
        return new HelpCommand();
    return nullptr;
}

// ===== LoadImagesCommand =====
LoadImagesCommand::LoadImagesCommand(const MyVector<MyString>& args) {
    for (size_t i = 0; i < args.size(); i++)
        paths.push_back(args[i]);
}
void LoadImagesCommand::execute() {
    for (size_t i = 0; i < paths.size(); i++)
        ImageList::getInstance().add(paths[i]);
}

// ===== RemoveImageCommand =====
RemoveImageCommand::RemoveImageCommand(const MyVector<MyString>& args) {
    name = args[0];
}
void RemoveImageCommand::execute() {
    ImageList::getInstance().remove(name);
}

// ===== PrintAllCommand =====
void PrintAllCommand::execute() {
    ImageList::getInstance().printAll();
}

// ===== AddFilterCommand =====
AddFilterCommand::AddFilterCommand(const MyVector<MyString>& args) {
    imageName = args[0];
    filterName = args[1];
}
void AddFilterCommand::execute() {
    Image* image = ImageList::getInstance()[imageName];
    if (!image) {
        std::cout << "Image not found\n";
        return;
    }
    image->addFilter(filterName);
}

// ===== RemoveFilterCommand =====
RemoveFilterCommand::RemoveFilterCommand(const MyVector<MyString>& args) {
    imageName = args[0];
    filterIndex = std::atoi(args[1].c_str());
}
void RemoveFilterCommand::execute() {
    Image* image = ImageList::getInstance()[imageName];
    if (!image) {
        std::cout << "Image not found\n";
        return;
    }
    image->removeFilter(filterIndex);
}

// ===== RunCommand =====
RunCommand::RunCommand(const MyVector<MyString>& args) {
    imageName = args[0];
}
void RunCommand::execute() {
    Image* image = ImageList::getInstance()[imageName];
    if (!image) {
        std::cout << "Image not found\n";
        return;
    }
    image->applyFilters();
}

// ===== RunAllCommand =====
void RunAllCommand::execute() {
    MyVector<MyString> images = ImageList::getInstance().getImageList();
    for (size_t i = 0; i < images.size(); i++) {
        Image* image = ImageList::getInstance()[images[i]];
        if (image) image->applyFilters();
    }
}

// ===== ShowFiltersCommand =====
ShowFiltersCommand::ShowFiltersCommand(const MyVector<MyString>& args) {
    name = args[0];
}
void ShowFiltersCommand::execute() {
    Image* image = ImageList::getInstance()[name];
    if (!image) {
        std::cout << "Image not found\n";
        return;
    }
    image->showFilters();
}

// ===== ShowAllFiltersCommand =====
void ShowAllFiltersCommand::execute() {
    MyVector<MyString> images = ImageList::getInstance().getImageList();
    for (size_t i = 0; i < images.size(); i++) {
        Image* image = ImageList::getInstance()[images[i]];
        if (image) image->showFilters();
    }
}

// ===== SaveImageCommand =====
SaveImageCommand::SaveImageCommand(const MyVector<MyString>& args) {
    name = args[0];
    if (args.size() >= 2)
        savePath = args[1];
}
void SaveImageCommand::execute() {
    Image* image = ImageList::getInstance()[name];
    if (!image) {
        std::cout << "Image not found\n";
        return;
    }
    if (savePath.empty())
        image->save(image->getOutputName());
    else
        image->save(savePath);
}

// ===== HelpCommand =====
void HelpCommand::execute() {
    std::cout << "Available commands:\n";
    std::cout << "  load <path1> <path2> ... - Load images from files\n";
    std::cout << "  remove <name> - Remove an image\n";
    std::cout << "  printAll - Show all loaded images\n";
    std::cout << "  add-filter <image> <filter> - Add a filter to an image\n";
    std::cout << "  remove-filter <image> <index> - Remove a filter by index\n";
    std::cout << "  show-filters <image> - Show filters of the image\n";
    std::cout << "  show-all-filters - Show filters of all images\n";
    std::cout << "  run <image> - Apply filters to the image\n";
    std::cout << "  run-all - Apply filters to all images\n";
    std::cout << "  save <image> [path] - Save an image\n";
    std::cout << "  help - Show this help message\n";
    std::cout << "  quit - Exit the program\n";
}
