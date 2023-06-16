#include <iostream>
#include "Tape.h"
#include "TapeSorter.h"

int main()
{
//    TapeEmulationSettings settings{};
//    Tape tape("tape.bin", settings);
//    std::cout << tape.getCurrentPosition() << ": " << tape.readCell() << std::endl;
//    tape.moveForward();
//    tape.writeCell(11111);
//    std::cout << tape.getCurrentPosition() << ": " << tape.readCell() << std::endl;
//    tape.writeCell(2222);
//    tape.readCell();
//    std::cout << tape.getCurrentPosition() << ": " << tape.readCell() << std::endl;
//    tape.moveBackwards();
//    std::cout << tape.getCurrentPosition() << ": " << tape.readCell() << std::endl;
//    tape.skip(-1);
//    tape.moveForward();
//    std::cout << tape.getCurrentPosition() << ": " << tape.readCell() << std::endl;
//    tape.moveForward();
//    tape.writeCell(142347278);
//    std::cout << tape.getCurrentPosition() << ": " << tape.readCell() << std::endl;
    TapeSorter ts = TapeSorter("input.bin", "output.bin");
    return 0;
}
