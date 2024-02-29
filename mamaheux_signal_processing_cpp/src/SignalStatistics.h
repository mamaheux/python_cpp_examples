#ifndef MAMAHEUX_SIGNAL_PROCESSING_CPP_SIGNAL_STATISTICS_H
#define MAMAHEUX_SIGNAL_PROCESSING_CPP_SIGNAL_STATISTICS_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <unordered_map>

class SignalStatistics
{
    float m_min;
    float m_max;
    float m_mean;

public:
    SignalStatistics(float min, float max, float mean) : m_min(min), m_max(max), m_mean(mean) {}

    float min() const { return m_min; }
    float max() const { return m_max; }
    float mean() const { return m_mean; }
};

void initSignalStatisticsPythonModule(pybind11::module& m);

#endif
