#include "FileUtility.h"

std::vector<char> FileUtility::ParseFile(const std::string& fileName){

    std::string filepath = std::string(ShaderDirectory) + fileName;

    std::fstream file(filepath, std::ios::ate | std::ios::binary);

    if(!file.is_open()){
        throw std::runtime_error("Failed to open file");
    }

    std::vector<char> buffer(file.tellg());

    file.seekg(0, std::ios::beg);
    file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));

    file.close();

    return buffer;
}
