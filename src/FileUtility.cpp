#include "FileUtility.h"

#include "Logs.h"

std::vector<char> FileUtility::ParseShaderFile(const std::string& filePath){

    FILE* file = fopen(filePath.c_str(), "rb");

    if(!file){
        throw std::runtime_error("Failed to open shader: " + filePath);
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    std::vector<char> buffer(size);

    if(fread((buffer.data()), 1, size, file) != size)
    {
        fclose(file);
        throw std::runtime_error("Incomplete shader read");
    }

    fclose(file);

    return buffer;
}
