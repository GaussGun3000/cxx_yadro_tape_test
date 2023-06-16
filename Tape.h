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

class Tape : public TapeInterface {

public:
    Tape(const std::string &inputFileName, const std::string &outputFileName, const TapeEmulationSettings &settings);

private:
    std::unique_ptr<TapeEmulationSettings> emulationSettings;
    std::fstream tapeFile;

    int32_t readCell() override;

public:
    void moveForward() override;

    void moveBackwards() override;

    void skip(int32_t cells) override;

    void writeCell(int32_t value) override;

    int64_t getCurrentPosition() override;

    void setReadLatency(uint32_t latency) override;

    void setWriteLatency(uint32_t latency) override;

    void setMoveLatency(uint32_t latency) override;

};


#endif //YADRO_TAPE_TEST_TAPE_H
