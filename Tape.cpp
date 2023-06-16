#include "Tape.h"

Tape::Tape(const std::string& inputFileName, const std::string& outputFileName, const TapeEmulationSettings& settings)
        : emulationSettings(std::make_unique<TapeEmulationSettings>(settings))
{
    tapeFile.open(inputFileName, std::ios::binary | std::ios::in | std::ios::out);
    if (!tapeFile.is_open())
    {
        std::cerr << "Failed to open input file: " << inputFileName << std::endl;
    }
}

int32_t Tape::readCell()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(emulationSettings->readLatency));

    int32_t value = 0;
    tapeFile.read(reinterpret_cast<char*>(&value), sizeof(int32_t));

    return value;
}

void Tape::writeCell(int32_t value)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(emulationSettings->writeLatency));
    tapeFile.write(reinterpret_cast<const char*>(&value), sizeof(int32_t));
}

void Tape::moveForward()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(emulationSettings->moveLatency));
    tapeFile.seekg(sizeof(int32_t), std::ios::cur);
}

void Tape::moveBackwards()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(emulationSettings->moveLatency));
    tapeFile.seekg(-static_cast<std::streamoff>(sizeof(int32_t)), std::ios::cur);
}

void Tape::skip(int32_t cells)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(emulationSettings->skipLatencyperCell * cells));
    tapeFile.seekg(cells * sizeof(int32_t), std::ios::cur);
}

int64_t Tape::getCurrentPosition()
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

