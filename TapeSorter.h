#ifndef YADRO_TAPE_TEST_TAPESORTER_H
#define YADRO_TAPE_TEST_TAPESORTER_H

#include "interface.h"
#include "Tape.h"
#include <string>
#include <vector>
#include <map>

class TapeSorter
{
public:
    TapeSorter(const std::string& inputFileName, const std::string& outputFileName);
    void sort();
    void printSorted();

private:
    std::unique_ptr<Tape> createEmptyTape(const std::string& fileName, uint32_t cellsCount);
    static bool createEmptyTapeFile(const std::string& fileName, uint64_t bytes);
    std::map<std::string, std::string> parseIniFile(const std::string & fileName);
    void multiFileSort();
    bool tempTapesAtEnd();
    void mergeTempTapes();

    TapeEmulationSettings emulationSettings;
    std::unique_ptr<Tape> inputTape;
    std::unique_ptr<Tape> outputTape;
    std::vector<std::unique_ptr<Tape>> tempTapes;
    uint32_t memoryLimit;
};


#endif //YADRO_TAPE_TEST_TAPESORTER_H
