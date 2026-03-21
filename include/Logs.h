#pragma once

namespace Logs{
    void Print(const char* log);
    void Print(int log);
    void Print(std::string_view log);

    void PrintError(const char* log);
    void PrintError(const char* log, const vk::SystemError& vkError);
}
