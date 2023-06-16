#ifndef YADRO_TAPE_TEST_INTERFACE_H
#define YADRO_TAPE_TEST_INTERFACE_H

#include <cstdint>

struct TapeEmulationSettings
{
   uint32_t readLatency;
   uint32_t writeLatency;
   uint32_t moveLatency;
   uint32_t skipLatencyPerCell;
};

class TapeInterface
{
public:
    virtual int32_t readCell() = 0;

    virtual void moveForward() = 0;

    virtual void moveBackwards() = 0;

    virtual void skip(int32_t beginning) = 0;

    virtual void writeCell(int32_t value) = 0;

    virtual uint32_t getCurrentPosition() = 0;

    virtual void setReadLatency(uint32_t latency) = 0;

    virtual void setWriteLatency(uint32_t latency) = 0;

    virtual void setMoveLatency(uint32_t latency) = 0;

    virtual ~TapeInterface() = default;
};

#endif //YADRO_TAPE_TEST_INTERFACE_H


