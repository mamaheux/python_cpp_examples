#ifndef MAMAHEUX_SIGNAL_PROCESSING_CPP_EFFECTS_H
#define MAMAHEUX_SIGNAL_PROCESSING_CPP_EFFECTS_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

pybind11::array_t<float> applyCompressor(
    const pybind11::array_t<float>& signal,
    float threshold,
    float ratio,
    float attack = 0.995f,
    float release = 0.9997f);

void initEffectsPythonModule(pybind11::module& m);

#endif
