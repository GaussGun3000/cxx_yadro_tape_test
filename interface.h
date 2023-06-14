#ifndef YADRO_TAPE_TEST_INTERFACE_H
#define YADRO_TAPE_TEST_INTERFACE_H

class Tape {
public:
    virtual void sort() = 0;

    virtual int32_t readCell() = 0;

    virtual void moveForward() = 0;

    virtual void moveBackwards() = 0;

    virtual void writeCell(int32_t value) = 0;

    virtual void setReadLatency(uint32_t latency) = 0;

    virtual void setWriteLatency(uint32_t latency) = 0;

    virtual void setMoveLatency(uint32_t latency) = 0;

    virtual void setMemoryLimit(uint32_t maxCellsInMemory) = 0;
};

#endif //YADRO_TAPE_TEST_INTERFACE_H


