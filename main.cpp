#include <iostream>
#include "Tape.h"
#include "TapeSorter.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Not enough arguments were provided!\n";
        return -1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    TapeSorter ts = TapeSorter(inputFile, outputFile);
    ts.sort();
    ts.printSorted();
    return 0;
}
