#include "Effects.h"

namespace py = pybind11;

py::array_t<float>
    applyCompressor(const py::array_t<float>& input, float threshold, float ratio, float attack, float release)
{
    if (input.ndim() != 1)
    {
        throw py::value_error("The signal should have only one dimension.");
    }

    py::array_t<float> output(input.shape(0));

    float attackComplement = 1.f - attack;
    float releaseComplement = 1.f - release;
    float ratioComplement = 1.f - ratio;
    float level = 0.f;
    auto inputUnchecked = input.unchecked();
    auto outputUnchecked = output.mutable_unchecked();

    for (py::ssize_t i = 0; i < input.shape(0); i++)
    {
        float value = inputUnchecked(i);
        float absValue = std::abs(value);

        // Update the level
        if (absValue > level)
        {
            level = attack * level + attackComplement * absValue;
        }
        else
        {
            level = release * level + releaseComplement * absValue;
        }

        // Compute the output value
        if (level > threshold)
        {
            outputUnchecked(i) = (ratio * threshold / level + ratioComplement) * value;
        }
        else
        {
            outputUnchecked(i) = value;
        }
    }

    return output;
}

void initEffectsPythonModule(py::module& m)
{
    m.def(
        "apply_compressor",
        &applyCompressor,
        "Apply a compressor effect."
        "\n"
        ":param input: The 1D signal to process\n"
        ":param threshold: The threshold value\n"
        ":param ratio: The threshold value\n"
        ":param attack: The threshold value\n"
        ":param release: The threshold value\n",
        py::arg("input"),
        py::arg("threshold"),
        py::arg("ratio"),
        py::arg("attack") = 0.995f,
        py::arg("release") = 0.9997f);
}
