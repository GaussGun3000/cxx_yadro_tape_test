#include "TapeSorter.h"
#include "TapeEmulator.h"
#include <memory>
#include <cassert>
#include <iostream>

void runTest(const std::string& inputFilePath, const std::string& outputFilePath, uint32_t memoryLimit)
{
    std::shared_ptr<TapeEmulator> inputTape = std::make_shared<TapeEmulator>(inputFilePath);
    std::shared_ptr<TapeEmulator> outputTape = std::make_shared<TapeEmulator>(outputFilePath, inputTape->getSize());

    TapeSorter<TapeEmulator> sorter = TapeSorter<TapeEmulator>(inputTape, outputTape, memoryLimit);
    sorter.sort();

    outputTape->skip(1);  // Reset tape to the start
    int32_t prevValue = outputTape->readCell();
    outputTape->moveForward();

    while (!outputTape->atEnd())
    {
        int32_t currentValue = outputTape->readCell();
        assert(prevValue <= currentValue);  // Check that the output is sorted
        prevValue = currentValue;
        outputTape->moveForward();
    }

    std::cout << "Test passed for: " << inputFilePath << std::endl;
}

int main()
{
    const uint32_t memoryLimit = 2000;

    // Test 1: Tape that fits into memory
    std::cout << "Running Test 1 (Fits into memory)..." << std::endl;
    system("./generator tape1.bin 500");
    runTest("tape1.bin", "sorted_tape1.bin", memoryLimit);

    // Test 2: Tape that is 5 times the memory size
    std::cout << "Running Test 2 (5 times the memory size)..." << std::endl;
    system("./generator tape2.bin 10000");
    runTest("tape2.bin", "sorted_tape2.bin", memoryLimit);

    // Test 3: Tape that is not an integer multiple of memory size
    std::cout << "Running Test 3 (Not integer multiple of memory size)..." << std::endl;
    system("./generator tape3.bin 4601");
    runTest("tape3.bin", "sorted_tape3.bin", memoryLimit);

    return 0;
}
