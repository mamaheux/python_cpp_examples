#include "SignalStatistics.h"
#include "Effects.h"

#include <pybind11/pybind11.h>

#define MACRO_STRINGIFY(x) #x

PYBIND11_MODULE(mamaheux_signal_processing_cpp, m)
{
    initSignalStatisticsPythonModule(m);
    initEffectsPythonModule(m);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
