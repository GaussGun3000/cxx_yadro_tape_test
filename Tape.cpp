#include "Tape.h"
#include <filesystem>

Tape::Tape(const std::string& fileName, const TapeEmulationSettings& settings)
        : emulationSettings(std::make_unique<TapeEmulationSettings>(settings))
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

int32_t Tape::readCell()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(emulationSettings->readLatency));
    if (getCurrentPosition() >= tapeSize)
    {
        throw std::out_of_range("Cannot write past end of Tape!");
    }

    int32_t value = 0;
    tapeFile.read(reinterpret_cast<char*>(&value), sizeof(int32_t));
    tapeFile.seekg(-static_cast<std::streamoff>(sizeof(int32_t)), std::ios::cur);

    return value;
}

void Tape::writeCell(int32_t value)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(emulationSettings->writeLatency));
    if (getCurrentPosition() >= tapeSize)
    {
        throw std::out_of_range("Cannot write past end of Tape!");
    }

    tapeFile.write(reinterpret_cast<const char*>(&value), sizeof(int32_t));
    tapeFile.seekp(-static_cast<std::streamoff>(sizeof(int32_t)), std::ios::cur);
}

void Tape::moveForward()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(emulationSettings->moveLatency));
    if (getCurrentPosition() >= tapeSize)
    {
        std::cout << "ERROR: Cannot move forward. Already at the end of the file." << std::endl;
        return;
    }
    else tapeFile.seekg(sizeof(int32_t), std::ios::cur);
}

void Tape::moveBackwards()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(emulationSettings->moveLatency));
    if (getCurrentPosition() == 0)
    {
        std::cout << "ERROR: Cannot move backwards. Already at the start of the file." << std::endl;
        return;
    }
    else tapeFile.seekg(-static_cast<std::streamoff>(sizeof(int32_t)), std::ios::cur);
}

void Tape::skip(int32_t beginning)
{
    switch (beginning)
    {
        case -1:
        {
            uint32_t cells = tapeSize - getCurrentPosition() / sizeof(int32_t);
            std::this_thread::sleep_for(std::chrono::milliseconds(emulationSettings->skipLatencyPerCell * cells));
            tapeFile.seekg(0, std::ios::end);
            break;
        }
        case 1:
        {
            uint32_t cells = getCurrentPosition() / sizeof(int32_t);
            std::this_thread::sleep_for(std::chrono::milliseconds(emulationSettings->skipLatencyPerCell * cells));
            tapeFile.seekg(0, std::ios::beg);
            break;
        }
        default:
            std::cerr << "Invalid argument for Tape::skip()" << std::endl;
            break;

    }
}

uint32_t Tape::getCurrentPosition()
{
    return tapeFile.tellg();
}

void Tape::setReadLatency(uint32_t latency)
{
    emulationSettings->readLatency = latency;
}

void Tape::setWriteLatency(uint32_t latency)
{
    emulationSettings->writeLatency = latency;
}

void Tape::setMoveLatency(uint32_t latency)
{
    emulationSettings->moveLatency = latency;
}

Tape::~Tape()
{
    if(tapeFile.is_open())
        tapeFile.close();
}

