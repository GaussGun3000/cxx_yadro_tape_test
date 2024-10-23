#include <iostream>
#include "TapeEmulator.h"
#include "TapeSorter.h"

int main(int argc, char* argv[])
{
    try
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
    }catch (std::exception& e)
    {
        std::cout << "Something went wrong: " << e.what() << std::endl;
    }
    std::cout << "Press enter to exit" << std::endl;
    getchar();
    return 0;
}
