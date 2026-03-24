#pragma once

namespace FileUtility{

    //shader file directory
    inline constexpr const char* ShaderDirectory = "Assets/shaders/";

    std::vector<char> ParseShaderFile(const std::string& filePath);

    inline std::string GetFileName(std::filesystem::path file) { return file.filename().string(); }

};
