#include "TapeEmulator.h"
#include <filesystem>

TapeEmulator::TapeEmulator(const std::string& fileName)
{
    tapeFile.open(fileName, std::ios::binary | std::ios::in | std::ios::out);
    if (!tapeFile.is_open())
    {
        std::cerr << "Failed to open input file: " << fileName << std::endl;
    }
    else
    {
        tapeSize = std::filesystem::file_size(fileName);
    }
}

int32_t TapeEmulator::readCell()
{
    std::this_thread::sleep_for(std::chrono::microseconds(emulationSettings->readLatency));
    if (getCurrentPosition() >= tapeSize)
    {
        throw std::out_of_range("Cannot read past end of Tape! CurrentPos:" + std::to_string(getCurrentPosition())
                                                                            + "Size: " + std::to_string(getSize()));
    }

    int32_t value = 0;
    tapeFile.read(reinterpret_cast<char*>(&value), sizeof(int32_t));
    tapeFile.seekg(-static_cast<std::streamoff>(sizeof(int32_t)), std::ios::cur);

    return value;
}

void TapeEmulator::writeCell(int32_t value)
{
    std::this_thread::sleep_for(std::chrono::microseconds(emulationSettings->writeLatency));
    if (getCurrentPosition() >= tapeSize)
    {
        throw std::out_of_range("Cannot write past end of Tape! CurrentPos:" + std::to_string(getCurrentPosition())
                                + "Size: " + std::to_string(getSize()));
    }

    tapeFile.write(reinterpret_cast<const char*>(&value), sizeof(int32_t));
    tapeFile.seekp(-static_cast<std::streamoff>(sizeof(int32_t)), std::ios::cur);
}

void TapeEmulator::moveForward()
{
    std::this_thread::sleep_for(std::chrono::microseconds(emulationSettings->moveLatency));
    if (getCurrentPosition() >= tapeSize)
    {
        std::cout << "ERROR: Cannot move forward. Already at the end of the file." << std::endl;
        return;
    }
    else tapeFile.seekg(sizeof(int32_t), std::ios::cur);
}

void TapeEmulator::moveBackwards()
{
    std::this_thread::sleep_for(std::chrono::microseconds(emulationSettings->moveLatency));
    if (getCurrentPosition() == 0)
    {
        std::cout << "ERROR: Cannot move backwards. Already at the start of the file." << std::endl;
        return;
    }
    else tapeFile.seekg(-static_cast<std::streamoff>(sizeof(int32_t)), std::ios::cur);
}

void TapeEmulator::skip(int32_t beginning)
{
    switch (beginning)
    {
        case -1:
        {
            uint32_t cells = tapeSize - getCurrentPosition() / sizeof(int32_t);
            std::this_thread::sleep_for(std::chrono::microseconds (emulationSettings->skipLatencyPerCell * cells));
            tapeFile.seekg(0, std::ios::end);
            break;
        }
        case 1:
        {
            uint32_t cells = getCurrentPosition() / sizeof(int32_t);
            std::this_thread::sleep_for(std::chrono::microseconds(emulationSettings->skipLatencyPerCell * cells));
            tapeFile.seekg(0, std::ios::beg);
            break;
        }
        default:
            std::cerr << "Invalid argument for Tape::skip()" << std::endl;
            break;

    }
}

uint32_t TapeEmulator::getCurrentPosition()
{
    return tapeFile.tellg();
}

void TapeEmulator::setReadLatency(uint32_t latency)
{
    emulationSettings->readLatency = latency;
}

void TapeEmulator::setWriteLatency(uint32_t latency)
{
    emulationSettings->writeLatency = latency;
}

void TapeEmulator::setMoveLatency(uint32_t latency)
{
    emulationSettings->moveLatency = latency;
}

TapeEmulator::~TapeEmulator()
{
    if(tapeFile.is_open())
        tapeFile.close();
}

uint32_t TapeEmulator::getSize() const
{
    return tapeSize;
}

bool TapeEmulator::atEnd()
{
    return getCurrentPosition() == tapeSize;
}

void TapeEmulator::printContentNoLatency()
{
    std::streampos currentPosition = tapeFile.tellg();
    tapeFile.seekg(0, std::ios::beg);

    int32_t value;
    while (tapeFile.read(reinterpret_cast<char*>(&value), sizeof(int32_t)))
    {
        std::cout << value << "\n";
    }
    tapeFile.seekg(currentPosition);
}

std::map<std::string, std::string> TapeEmulator::parseIniFile(const std::string &fileName)
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
