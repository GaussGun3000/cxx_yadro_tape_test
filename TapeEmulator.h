//
// Created by vva07 on 15.06.2023.
//

#ifndef YADRO_TAPE_TEST_TAPE_H
#define YADRO_TAPE_TEST_TAPE_H

#include "interface.h"
#include <memory>
#include <fstream>

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <cstdint>
#include <map>

struct TapeEmulationSettings
{
   uint32_t readLatency;
   uint32_t writeLatency;
   uint32_t moveLatency;
   uint32_t skipLatencyPerCell;
};

class TapeEmulator : public TapeInterface
{
public:
    TapeEmulator(const std::string &fileName);
    uint32_t getSize() const override;
    bool atEnd() const override;

    int32_t readCell() override;
    void moveForward() override;
    void moveBackwards() override;
    void skip(int32_t beginning) override;
    void writeCell(int32_t value) override;
    uint32_t getCurrentPosition() override;
    ~TapeEmulator() override;

    // METHODS FOR SIMULATION
    void setReadLatency(uint32_t latency);
    void setWriteLatency(uint32_t latency);
    void setMoveLatency(uint32_t latency);
    void printContentNoLatency();

private:
    // METHODS
    std::map<std::string, std::string> parseIniFile(const std::string &fileName);

    // MEMBERS
    std::unique_ptr<TapeEmulationSettings> emulationSettings;
    std::fstream tapeFile;
    uint32_t tapeSize;

};


#endif //YADRO_TAPE_TEST_TAPE_H
