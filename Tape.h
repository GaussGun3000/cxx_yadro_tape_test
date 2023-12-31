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

class Tape : public TapeInterface
{
public:
    Tape(const std::string &fileName, const TapeEmulationSettings &settings);
    uint32_t getSize() const;
    bool atEnd();
    void printContentNoLatency();

    int32_t readCell() override;

    void moveForward() override;

    void moveBackwards() override;

    void skip(int32_t beginning) override;

    void writeCell(int32_t value) override;

    uint32_t getCurrentPosition() override;

    void setReadLatency(uint32_t latency) override;

    void setWriteLatency(uint32_t latency) override;

    void setMoveLatency(uint32_t latency) override;

    ~Tape() override;

private:
    std::unique_ptr<TapeEmulationSettings> emulationSettings;
    std::fstream tapeFile;
    uint32_t tapeSize;

};


#endif //YADRO_TAPE_TEST_TAPE_H
