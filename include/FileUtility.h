#pragma once

namespace FileUtility{

    //shader file directory
    inline constexpr const char* ShaderDirectory = "Assets/shaders/";

    std::vector<char> ParseFile(const std::string& fileName);

};
