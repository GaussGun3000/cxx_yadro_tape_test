#include "TapeEmulator.h"
#include <filesystem>



TapeEmulator::TapeEmulator(const std::string& fileName)
{
    m_tapeFile.open(fileName, std::ios::binary | std::ios::in | std::ios::out);
    if (!m_tapeFile.is_open())
        std::cerr << "Failed to open input file: " << fileName << std::endl;
    else
    {
        parseConfig("../config.ini");
        m_tapeSize = std::filesystem::file_size(fileName);

    }
}

TapeEmulator::TapeEmulator(const std::string& fileName, uint32_t tapeSize)
{
    createEmptyTapeFile(fileName, tapeSize);
    m_tapeFile.open(fileName, std::ios::binary | std::ios::in | std::ios::out);
    if (!m_tapeFile.is_open())
        std::cerr << "Failed to open input file: " << fileName << std::endl;
    else
    {
        parseConfig("../config.ini");
        m_tapeSize = std::filesystem::file_size(fileName);

    }
}

int32_t TapeEmulator::readCell()
{
    std::this_thread::sleep_for(std::chrono::microseconds(m_emulationSettings.readLatency));
    if (getCurrentPosition() >= m_tapeSize)
    {
        throw std::out_of_range("Cannot read past end of Tape! CurrentPos:" + std::to_string(getCurrentPosition())
                                                                            + " Size: " + std::to_string(getSize()));
    }

    int32_t value = 0;
    m_tapeFile.read(reinterpret_cast<char*>(&value), sizeof(int32_t));
    m_tapeFile.seekg(-static_cast<std::streamoff>(sizeof(int32_t)), std::ios::cur);

    return value;
}

void TapeEmulator::writeCell(int32_t value)
{
    std::this_thread::sleep_for(std::chrono::microseconds(m_emulationSettings.writeLatency));
    if (getCurrentPosition() >= m_tapeSize)
    {
        throw std::out_of_range("Cannot write past end of Tape! CurrentPos:" + std::to_string(getCurrentPosition())
                                + " Size: " + std::to_string(getSize()));
    }

    m_tapeFile.write(reinterpret_cast<const char*>(&value), sizeof(int32_t));
    m_tapeFile.seekp(-static_cast<std::streamoff>(sizeof(int32_t)), std::ios::cur);
}

void TapeEmulator::moveForward()
{
    std::this_thread::sleep_for(std::chrono::microseconds(m_emulationSettings.moveLatency));
    if (getCurrentPosition() >= m_tapeSize)
    {
        std::cout << "ERROR: Cannot move forward. Already at the end of the file." << std::endl;
        return;
    }
    else m_tapeFile.seekg(sizeof(int32_t), std::ios::cur);
}

void TapeEmulator::moveBackwards()
{
    std::this_thread::sleep_for(std::chrono::microseconds(m_emulationSettings.moveLatency));
    if (getCurrentPosition() == 0)
    {
        std::cout << "ERROR: Cannot move backwards. Already at the start of the file." << std::endl;
        return;
    }
    else m_tapeFile.seekg(-static_cast<std::streamoff>(sizeof(int32_t)), std::ios::cur);
}

void TapeEmulator::skip(int32_t beginning)
{
    switch (beginning)
    {
        case -1:
        {
            uint32_t cells = m_tapeSize - getCurrentPosition() / sizeof(int32_t);
            std::this_thread::sleep_for(std::chrono::microseconds (m_emulationSettings.skipLatencyPerCell * cells));
            m_tapeFile.seekg(0, std::ios::end);
            break;
        }
        case 1:
        {
            uint32_t cells = getCurrentPosition() / sizeof(int32_t);
            std::this_thread::sleep_for(std::chrono::microseconds(m_emulationSettings.skipLatencyPerCell * cells));
            m_tapeFile.seekg(0, std::ios::beg);
            break;
        }
        default:
            std::cerr << "Invalid argument for Tape::skip()" << std::endl;
            break;

    }
}

uint32_t TapeEmulator::getCurrentPosition()
{
    return m_tapeFile.tellg();
}

void TapeEmulator::setReadLatency(uint32_t latency)
{
    m_emulationSettings.readLatency = latency;
}

void TapeEmulator::setWriteLatency(uint32_t latency)
{
    m_emulationSettings.writeLatency = latency;
}

void TapeEmulator::setMoveLatency(uint32_t latency)
{
    m_emulationSettings.moveLatency = latency;
}

TapeEmulator::~TapeEmulator()
{
    if(m_tapeFile.is_open())
        m_tapeFile.close();
}

uint32_t TapeEmulator::getSize() const
{
    return m_tapeSize;
}

bool TapeEmulator::atEnd()
{
    return getCurrentPosition() == m_tapeSize;
}

void TapeEmulator::printContentNoLatency()
{
    std::streampos currentPosition = m_tapeFile.tellg();
    m_tapeFile.seekg(0, std::ios::beg);

    int32_t value;
    while (m_tapeFile.read(reinterpret_cast<char*>(&value), sizeof(int32_t)))
    {
        std::cout << value << "\n";
    }
    m_tapeFile.seekg(currentPosition);
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

void TapeEmulator::parseConfig(const std::string& configFile)
{
    try
    {
        auto config = parseIniFile(configFile);

        m_emulationSettings.readLatency = std::stoi(config["readLatency"]);
        m_emulationSettings.writeLatency = std::stoi(config["writeLatency"]);
        m_emulationSettings.moveLatency = std::stoi(config["moveLatency"]);
        m_emulationSettings.skipLatencyPerCell = std::stoi(config["skipLatencyPerCell"]);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Could not parse configuration file: " << e.what() << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Could not parse configuration file: " << e.what() << std::endl;
    }
}

bool TapeEmulator::createEmptyTapeFile(const std::string &fileName, uint32_t bytes)
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
