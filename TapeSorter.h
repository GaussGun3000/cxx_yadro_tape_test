#ifndef YADRO_TAPE_TEST_TAPESORTER_H
#define YADRO_TAPE_TEST_TAPESORTER_H

#include "interface.h"
#include "TapeEmulator.h"
#include <string>
#include <vector>
#include <map>

template <typename T>
class TapeSorter
{
public:
    // Constructor takes shared pointers to input and output tapes
    TapeSorter(std::shared_ptr<T> inputTape, std::shared_ptr<T> outputTape, uint32_t memoryLimit);
    void sort();

private:
    // Helper method to init and add a temporary tape to m_tempTapes
    void getEmptyTempTape(uint32_t chunkIndex, uint32_t chunkSize);

    // Helper methods for multi-file sorting and merging temporary tapes
    void multiTapeSort();
    bool tempTapesAtEnd();
    void mergeTempTapes();
    bool currentValuesEmpty(std::vector<int32_t>& currentValues);

    // MEMBERS
    std::shared_ptr<TapeInterface> m_inputTape;  // Shared pointer to the input tape
    std::shared_ptr<TapeInterface> m_outputTape; // Shared pointer to the output tape
    std::vector<std::shared_ptr<TapeInterface>> m_tempTapes;  // Temp tapes used during sorting
    uint32_t m_memoryLimit;  // Memory limit for sorting
};


#endif //YADRO_TAPE_TEST_TAPESORTER_H
