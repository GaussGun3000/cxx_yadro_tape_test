#include <iostream>
#include "TapeEmulator.h"
#include "TapeSorter.h"
#include <sstream>
#include <string>
#include <fstream>
#include <map>

#include <fstream>
#include <iostream>
#include <cstdint>

void containsZeroes(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary | std::ios::in);
    uint32_t zeros = 0;
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    int32_t value;
    while (file.read(reinterpret_cast<char*>(&value), sizeof(int32_t))) {
        if (value == 0)
            zeros++;  // Found a zero
    }

    std::cout << filePath << ": " << zeros << " zeros found" << std::endl;
}


uint32_t getMemoryLimit(const std::string &fileName)
{
    std::map<std::string, std::string> config;

    std::ifstream file(fileName);
    if (!file)
    {
        std::cerr << "Failed to open configuration file: " << fileName << std::endl;
        return 0;
    }

    std::string line;
    std::string section;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string key, value;
        if (std::getline(ss, key, '=') && std::getline(ss, value))
        {

            key.erase(0, key.find_first_not_of(' '));
            key.erase(key.find_last_not_of(' ') + 1);
            value.erase(0, value.find_first_not_of(' '));
            value.erase(value.find_last_not_of(' ') + 1);

            if (!key.empty())
            {
                if (key.front() == '[' && key.back() == ']')
                {
                    section = key.substr(1, key.size() - 2);
                }
                else
                {
                    std::string fullKey = section.empty() ? key : (section + "." + key);
                    config[fullKey] = value;
                }
            }
        }
    }
    return  std::stoi(config["memoryLimit"]);
}


int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            std::cout << "Not enough arguments were provided!\nUsage: yadro_tape_test <inputFile> <outputFile>\n";
            return -1;
        }

        std::string inputFile = argv[1];
        std::string outputFile = argv[2];
        std::shared_ptr<TapeEmulator> inputTape = std::make_shared<TapeEmulator>(inputFile);
        std::shared_ptr<TapeEmulator> outputTape = std::make_shared<TapeEmulator>(outputFile, inputTape->getSize());

        TapeSorter<TapeEmulator> ts = TapeSorter<TapeEmulator>(inputTape, outputTape, getMemoryLimit("../config.ini"));
        ts.sort();
        std::cout << inputTape->getSize() << std::endl;
        std::cout << outputTape->getSize() << std::endl;
//        containsZeroes(inputFile);
//        containsZeroes("temp/temp_0");
//        containsZeroes("temp/temp_1");
//        containsZeroes("temp/temp_2");
//        containsZeroes("temp/temp_3");
        containsZeroes(outputFile);
//        outputTape->printContentNoLatency();
    }catch (std::exception& e)
    {
        std::cout << "Something went wrong: " << e.what() << std::endl;
    }
    std::cout << "Press enter to exit" << std::endl;
    getchar();
    return 0;
}
