#include "Logs.h"

void Logs::Print(const char *log){
    std::cout << log << "\n";
}

void Logs::Print(int log){
    std::cout << log << "\n";
}

void Logs::Print(std::string_view log){
    std::cout << log << "\n";
}

void Logs::PrintError(const char *log){
    std::cerr << log << "\n";
}

void Logs::PrintError(const char *log, const vk::SystemError& vkError){
    std::cerr << log << ": " << vkError.what() << std::endl;
}
