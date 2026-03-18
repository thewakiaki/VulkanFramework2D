#pragma once

namespace Logs{
    void Print(const char* log);

    void LogError(const char* log, const vk::SystemError& vkError);
}
