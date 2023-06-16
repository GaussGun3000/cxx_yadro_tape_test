#include <iostream>
#include "Tape.h"

int main()
{
    TapeEmulationSettings settings{};
    Tape tape("tape.bin", settings);
    std::cout << tape.readCell() << std::endl;
    tape.moveForward();
    return 0;
}
