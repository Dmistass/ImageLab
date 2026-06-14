#include <iostream>
#include <cstdlib>
#include "Commands.h"
#include "ImageList.h"
#include "FilterFactory.h"

// Takes a command name and argument list, creates and returns the corresponding command
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
    if (name == "show-available-filters" && args.size() == 0)
        return new ShowAvailableFiltersCommand();
    return nullptr;
}

// Saves all passed paths into paths
LoadImagesCommand::LoadImagesCommand(const MyVector<MyString>& args) {
    for (size_t i = 0; i < args.size(); i++)
        paths.push_back(args[i]);
}
// Loads each image
void LoadImagesCommand::execute() {
    for (size_t i = 0; i < paths.size(); i++)
        ImageList::getInstance().add(paths[i]);
}

RemoveImageCommand::RemoveImageCommand(const MyVector<MyString>& args) {
    name = args[0];
}
// Removes the image
void RemoveImageCommand::execute() {
    ImageList::getInstance().remove(name);
}

void PrintAllCommand::execute() {
    ImageList::getInstance().printAll();
}

// Takes an image name and a filter name
AddFilterCommand::AddFilterCommand(const MyVector<MyString>& args) {
    imageName = args[0];
    filterName = args[1];
}
// Finds the image by name and adds a filter to it
void AddFilterCommand::execute() {
    Image* image = ImageList::getInstance()[imageName];
    if (!image) {
        std::cout << "Image not found\n";
        return;
    }
    image->addFilter(filterName);
}

// Takes an image name and filter index (from show-filters)
RemoveFilterCommand::RemoveFilterCommand(const MyVector<MyString>& args) {
    imageName = args[0];
    filterIndex = std::atoi(args[1].c_str());  // convert string to number
}
void RemoveFilterCommand::execute() {
    Image* image = ImageList::getInstance()[imageName];
    if (!image) {
        std::cout << "Image not found\n";
        return;
    }
    image->removeFilter(filterIndex);
}

RunCommand::RunCommand(const MyVector<MyString>& args) {
    imageName = args[0];
}
// Applies all assigned filters to the specified image
void RunCommand::execute() {
    Image* image = ImageList::getInstance()[imageName];
    if (!image) {
        std::cout << "Image not found\n";
        return;
    }
    image->applyFilters();
}

// Applies filters to all loaded images
void RunAllCommand::execute() {
    MyVector<MyString> images = ImageList::getInstance().getImageList();
    for (size_t i = 0; i < images.size(); i++) {
        Image* image = ImageList::getInstance()[images[i]];
        if (image) image->applyFilters();
    }
}

// Stores the image name
ShowFiltersCommand::ShowFiltersCommand(const MyVector<MyString>& args) {
    name = args[0];
}
// Shows the filter list of the image
void ShowFiltersCommand::execute() {
    Image* image = ImageList::getInstance()[name];
    if (!image) {
        std::cout << "Image not found\n";
        return;
    }
    image->showFilters();
}

// Shows filters for each loaded image
void ShowAllFiltersCommand::execute() {
    MyVector<MyString> images = ImageList::getInstance().getImageList();
    for (size_t i = 0; i < images.size(); i++) {
        Image* image = ImageList::getInstance()[images[i]];
        if (image) image->showFilters();
    }
}

// Takes an image name and an optional save path
SaveImageCommand::SaveImageCommand(const MyVector<MyString>& args) {
    name = args[0];
    if (args.size() >= 2)
        savePath = args[1];  // if second argument is passed - it's the save path
}
// Saves the image: if savePath is not set - uses outputName
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

// Prints the list of all available commands with a brief description
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
    std::cout << "  show-available-filters - Show all available filter names\n";
    std::cout << "  help - Show this help message\n";
    std::cout << "  quit - Exit the program\n";
}

// Prints all filter names available in FilterFactory
void ShowAvailableFiltersCommand::execute() {
    MyVector<MyString> names = FilterFactory::getInstance().getAvailableFilterNames();
    std::cout << "Available filters:\n";
    for (size_t i = 0; i < names.size(); i++) {
        std::cout << "  - " << names[i] << "\n";
    }
}
