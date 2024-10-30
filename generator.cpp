#include <fstream>
#include <iostream>
#include <cstdint>
#include <random>

void generateTapeFile(const std::string& filePath, uint32_t numIntegers)
{
    std::ofstream file(filePath, std::ios::binary | std::ios::out);

    if (file.is_open())
    {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int32_t> distribution(-10000000, 10000000);

        for (uint32_t i = 0; i < numIntegers; ++i)
        {
            int32_t value = distribution(rng);
            file.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }

        file.close();
        std::cout << "Tape file generated successfully." << std::endl;
    } else {
        std::cout << "Failed to open the file." << std::endl;
    }
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <filePath> <numIntegers>" << std::endl;
        return 1;
    }

    const std::string filePath = argv[1];
    uint32_t numIntegers = std::stoi(argv[2]);
    generateTapeFile(filePath, numIntegers);
    return 0;
}
