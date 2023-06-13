#ifndef YADRO_TAPE_TEST_INTERFACE_H
#define YADRO_TAPE_TEST_INTERFACE_H

#endif //YADRO_TAPE_TEST_INTERFACE_H

class Tape {
public:
    // Read a record from the tape
    virtual bool readRecord(int& record) = 0;

    // Write a record to the tape
    virtual void writeRecord(int record) = 0;

    // Set the latency for reading a record from the tape
    virtual void setReadLatency(int latency) = 0;

    // Set the latency for writing a record to the tape
    virtual void setWriteLatency(int latency) = 0;

    // Set the latency for transposing the tape
    virtual void setTranspositionLatency(int latency) = 0;

    // Sort the tape using an external sorting algorithm
    virtual void sort() = 0;
};
