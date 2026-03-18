#include "Logs.h"

void Logs::Print(const char *log){
    std::cout << log << "\n";
}

void Logs::LogError(const char *log, const vk::SystemError& vkError){
    std::cerr << log << ": " << vkError.what() << std::endl;
}
