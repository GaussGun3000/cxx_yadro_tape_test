#ifndef YADRO_TAPE_TEST_INTERFACE_H
#define YADRO_TAPE_TEST_INTERFACE_H

#include <cstdint>

struct TapeEmulationSettings
{
   uint32_t readLatency;
   uint32_t writeLatency;
   uint32_t moveLatency;
   uint32_t skipLatencyperCell;
};

class TapeInterface {
public:
    virtual void sort();

    virtual int32_t readCell();

    virtual void moveForward();

    virtual void moveBackwards();

    virtual void skip(int32_t cells);

    virtual void writeCell(int32_t value);

    virtual int64_t getCurrentPosition();

    virtual void setReadLatency(uint32_t latency);

    virtual void setWriteLatency(uint32_t latency);

    virtual void setMoveLatency(uint32_t latency);

    virtual ~TapeInterface() = default;
};

#endif //YADRO_TAPE_TEST_INTERFACE_H


