#include <sstream>
#include <algorithm>
#include "TapeSorter.h"
#include "TapeEmulator.h"

template class TapeSorter<TapeEmulator>;

template <typename T>
TapeSorter<T>::TapeSorter(std::shared_ptr<T> inputTape, std::shared_ptr<T> outputTape, uint32_t memoryLimit):
    m_inputTape(inputTape), m_outputTape(outputTape), m_memoryLimit(memoryLimit)
{
}

template <>
void TapeSorter<TapeEmulator>::getEmptyTempTape(uint32_t chunkIndex, uint32_t chunkSize)
{
    std::string filePath = "temp/temp_" + std::to_string(chunkIndex);
    std::shared_ptr<TapeInterface> tempTape = std::make_shared<TapeEmulator>(filePath, chunkSize * 4);
    m_tempTapes.push_back(tempTape);
}

template <typename T>
void TapeSorter<T>::sort()
{
    std::cout << "started sorting" << std::endl;
    if (m_inputTape->getSize() <= m_memoryLimit)
    {
        std::vector<int32_t> data;
        int32_t value;
        while (!m_inputTape->atEnd())
        {
            value = m_inputTape->readCell();
            data.push_back(value);
            m_inputTape->moveForward();
        }
        std::sort(data.begin(), data.end());
        for (const auto& element : data)
        {
            m_outputTape->writeCell(element);
            m_outputTape->moveForward();
        }
    }
    else multiTapeSort();
    std::cout << "Sort finished" << std::endl;
}

template <typename T>
void TapeSorter<T>::multiTapeSort()
{
    const uint32_t chunkSize = m_memoryLimit / sizeof(int32_t);

    uint32_t chunkIndex = 0;
    int32_t value;
    std::vector<int32_t> chunkBuffer;

    while (!m_inputTape->atEnd())
    {
        value = m_inputTape->readCell();
        chunkBuffer.push_back(value);

        if (chunkBuffer.size() == chunkSize)
        {
            std::sort(chunkBuffer.begin(), chunkBuffer.end());
            getEmptyTempTape(chunkIndex, chunkSize);
            for (const auto &element: chunkBuffer)
            {
                m_tempTapes.at(chunkIndex)->writeCell(element);
                m_tempTapes.at(chunkIndex)->moveForward();
            }

            chunkIndex++;
            chunkBuffer.clear();
        }
        m_inputTape->moveForward();
    }

    if (!chunkBuffer.empty())
    {
        getEmptyTempTape(chunkIndex, chunkBuffer.size());
        std::sort(chunkBuffer.begin(), chunkBuffer.end());

        for (const auto& element : chunkBuffer)
        {
            m_tempTapes.at(chunkIndex)->writeCell(element);
            m_tempTapes.at(chunkIndex)->moveForward();
        }
    }

    for (const auto& tempTape : m_tempTapes)
        tempTape->skip(1);

    mergeTempTapes();

}

template <typename T>
bool TapeSorter<T>::tempTapesAtEnd()
{
    bool allTapesAtEnd = true;
    for(const auto& tape : m_tempTapes)
    {
        if (!tape->atEnd())
        {
            allTapesAtEnd = false;
            break;
        }
    }
    return allTapesAtEnd;
}

template <typename T>
bool TapeSorter<T>::currentValuesEmpty(std::vector<int32_t>& currentValues)
{
    for(const int32_t value: currentValues)
    {
        if (value != std::numeric_limits<int32_t>::max())
            return false;
    }
    return true;
}

template <typename T>
void TapeSorter<T>::mergeTempTapes()
{
    std::cout << "Merging...\n" ;
    std::vector<int32_t> currentValues(m_tempTapes.size());
    std::vector<bool> tapeActive(m_tempTapes.size(), true);

    for (size_t i = 0; i < m_tempTapes.size(); ++i)
    {
        if (!m_tempTapes[i]->atEnd())
        {
            currentValues.at(i) = m_tempTapes[i]->readCell();
            m_tempTapes.at(i)->moveForward();
        }
    }

    m_outputTape->skip(1);
    while(!tempTapesAtEnd() || !currentValuesEmpty(currentValues))
    {
        auto index = std::distance(currentValues.begin(),
                                   std::min_element(currentValues.begin(), currentValues.end()));

        if (currentValues.at(index) == 0)
            std::cout << " ";
        m_outputTape->writeCell(currentValues.at(index));
        m_outputTape->moveForward();
        if (!m_tempTapes.at(index)->atEnd())
        {
            currentValues.at(index) = m_tempTapes.at(index)->readCell();
            m_tempTapes.at(index)->moveForward();
        }
        else currentValues.at(index) = std::numeric_limits<int32_t>::max();
    }
}


