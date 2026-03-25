#pragma once

namespace Logs{
    void Print(const char* log);
    void Print(int log);
    void Print(std::string_view log);

    void PrintError(const char* log);
    void PrintError(std::string_view log);
    void PrintError(const char* log, const vk::SystemError& vkError);
    void PrintError(const char* log, const VkResult& result);

    void PrintComponentDestroyed(const char* component);
}
