#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace EngineVulkan
{
    static std::vector<char> ReadFile(const std::string &fileName)
    {
        std::ifstream file(fileName, std::ios::ate | std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file!");
        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }
}