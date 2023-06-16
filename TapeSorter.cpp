#include <sstream>
#include <algorithm>
#include "TapeSorter.h"

TapeSorter::TapeSorter(const std::string &inputFileName, const std::string &outputFileName)
{
    try
    {
        auto config = parseIniFile("../config.ini");
        emulationSettings.writeLatency = stoi(config.at("writeLatency"));
        emulationSettings.readLatency = stoi(config.at("readLatency"));
        emulationSettings.moveLatency = stoi(config.at("moveLatency"));
        emulationSettings.skipLatencyPerCell = stoi(config.at("skipLatencyPerCell"));
        memoryLimit = stoi(config.at("memoryLimit"));
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Could not parse configuration file: " << e.what() << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Could not parse configuration file: " << e.what() << std::endl;
    }
    inputTape = std::make_unique<Tape>(inputFileName, emulationSettings);
    outputTape = createEmptyTape(outputFileName, inputTape->getSize() / sizeof(int32_t));
}


std::unique_ptr<Tape> TapeSorter::createEmptyTape(const std::string &fileName, uint32_t cellsCount)
{
    if (createEmptyTapeFile(fileName, cellsCount * 4))
    {
        std::unique_ptr<Tape> tape = std::make_unique<Tape>(fileName, emulationSettings);
        return tape;
    }
    else return nullptr;
}

bool TapeSorter::createEmptyTapeFile(const std::string &fileName, uint64_t bytes)
{
    std::ofstream file(fileName, std::ios::binary | std::ios::trunc);
    if (!file)
    {
        std::cerr << "Failed to create file: " << fileName << std::endl;
        return false;
    }
    const char zeroByte = '\0';
    for (uint32_t i = 0; i < bytes; ++i)
    {
        file.write(&zeroByte, sizeof(char));
    }

    file.close();
    return true;
}

std::map<std::string, std::string> TapeSorter::parseIniFile(const std::string &fileName)
{
    std::map<std::string, std::string> config;

    std::ifstream file(fileName);
    if (!file)
    {
        std::cerr << "Failed to open configuration file: " << fileName << std::endl;
        return config;
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

    return config;

}

void TapeSorter::sort()
{
    std::cout << "started sorting" << std::endl;
    if (inputTape->getSize() <= memoryLimit)
    {
        std::vector<int32_t> data;
        int32_t value;
        while (!inputTape->atEnd())
        {
            value = inputTape->readCell();
            data.push_back(value);
            inputTape->moveForward();
        }
        std::sort(data.begin(), data.end());
        for (const auto& element : data)
        {
            outputTape->writeCell(element);
            outputTape->moveForward();
        }
    }
    else multiFileSort();
    std::cout << "Sort finished" << std::endl;
}

void TapeSorter::multiFileSort()
{
    const uint32_t chunkSize = memoryLimit / sizeof(int32_t);

    uint32_t chunkIndex = 0;
    int32_t value;
    std::vector<int32_t> chunkBuffer;

    while (!inputTape->atEnd())
    {
        value = inputTape->readCell();
        chunkBuffer.push_back(value);

        if (chunkBuffer.size() >= chunkSize)
        {
            std::sort(chunkBuffer.begin(), chunkBuffer.end());
            tempTapes.emplace_back(createEmptyTape(std::string("./temp/temp_" + std::to_string(chunkIndex)), chunkSize));

            for (const auto &element: chunkBuffer)
            {
                tempTapes.at(chunkIndex)->writeCell(element);
                tempTapes.at(chunkIndex)->moveForward();
            }

            chunkIndex++;
            chunkBuffer.clear();
        }
        inputTape->moveForward();
    }

    if (!chunkBuffer.empty())
    {
        std::sort(chunkBuffer.begin(), chunkBuffer.end());

        for (const auto& element : chunkBuffer)
        {
            tempTapes[chunkIndex]->writeCell(element);
            tempTapes[chunkIndex]->moveForward();
        }
    }

    for (const auto& tempTape : tempTapes)
        tempTape->skip(1);

    mergeTempTapes();

}

bool TapeSorter::tempTapesAtEnd()
{
    bool allTapesAtEnd = true;
    for(const auto& tape : tempTapes)
    {
        if (!tape->atEnd())
        {
            allTapesAtEnd = false;
            break;
        }
    }
    return allTapesAtEnd;
}

void TapeSorter::mergeTempTapes()
{
    std::cout << "Merging...\n" ;
    std::vector<int32_t> currentValues(tempTapes.size());
    for (size_t i = 0; i < tempTapes.size(); ++i)
    {
        if (!tempTapes[i]->atEnd())
        {
            currentValues.at(i) = tempTapes[i]->readCell();
            tempTapes.at(i)->moveForward();
        }
    }


    outputTape->skip(1);
    while(!tempTapesAtEnd())
    {
        auto index = std::distance(currentValues.begin(),
                                   std::min_element(currentValues.begin(), currentValues.end()));

        outputTape->writeCell(currentValues.at(index));
        outputTape->moveForward();
        if (!tempTapes.at(index)->atEnd())
        {
            currentValues.at(index) = tempTapes.at(index)->readCell();
            tempTapes.at(index)->moveForward();
        }
        else currentValues.at(index) = std::numeric_limits<int32_t>::max();
    }
}

void TapeSorter::printSorted()
{
    outputTape->printContentNoLatency();
}

