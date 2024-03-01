#include "SignalStatistics.h"

#include <pybind11/stl.h>

#include <limits>

#if defined(__AVX2__)

#include <immintrin.h>

#endif

#include <iostream>

namespace py = pybind11;

SignalStatistics computeSignalStatistics(const py::array_t<float>& signal)
{
    if (signal.ndim() != 1)
    {
        throw py::value_error("The signal should have only one dimension.");
    }

    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
    float mean = 0.f;

    auto signalUnchecked = signal.unchecked();
    for (py::ssize_t i = 0; i < signal.shape(0); i++)
    {
        float v = signalUnchecked(i);
        min = std::min(v, min);
        max = std::max(v, max);
        mean += v;
    }

    mean /= static_cast<float>(signal.shape(0));

    return {min, max, mean};
}

#if defined(__AVX2__)

float sum8(__m256 x)
{
    __m128 loQuad = _mm256_extractf128_ps(x, 0);
    __m128 hiQuad = _mm256_extractf128_ps(x, 1);
    __m128 sum4 = _mm_add_ps(loQuad, hiQuad);

    float* sum = reinterpret_cast<float*>(&sum4);
    return sum[0] + sum[1] + sum[2] + sum[3];
}

// Bug
float min8(__m256 x)
{
    __m128 loQuad = _mm256_extractf128_ps(x, 0);
    __m128 hiQuad = _mm256_extractf128_ps(x, 1);
    __m128 min4 = _mm_min_ps(loQuad, hiQuad);

    float* min = reinterpret_cast<float*>(&min4);
    return std::min(std::min(min[0], min[1]), std::min(min[2], min[3]));
}

float max8(__m256 x)
{
    __m128 loQuad = _mm256_extractf128_ps(x, 0);
    __m128 hiQuad = _mm256_extractf128_ps(x, 1);
    __m128 max4 = _mm_max_ps(loQuad, hiQuad);

    float* max = reinterpret_cast<float*>(&max4);
    return std::max(std::max(max[0], max[1]), std::max(max[2], max[3]));
}

SignalStatistics computeSignalStatisticsSimd(const py::array_t<float>& signal)
{
    constexpr size_t VECTOR_SIZE = 8;

    if (signal.ndim() != 1)
    {
        throw py::value_error("The signal should have only one dimension.");
    }

    const float* data = signal.data(0);
    size_t size = signal.shape(0);

    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
    float sum = 0;

    size_t startCount = (VECTOR_SIZE - ((reinterpret_cast<size_t>(data) / sizeof(float)) % VECTOR_SIZE)) % VECTOR_SIZE;

    size_t i = 0;
    for (; i < startCount; i++)
    {
        float v = data[i];
        min = std::min(v, min);
        max = std::max(v, max);
        sum += v;
    }

    if (size > VECTOR_SIZE)
    {
        __m256 minVector = _mm256_set1_ps(std::numeric_limits<float>::max());
        __m256 maxVector = _mm256_set1_ps(std::numeric_limits<float>::min());
        __m256 sumVector = _mm256_set1_ps(0.f);

        for (; i < (size - VECTOR_SIZE); i += VECTOR_SIZE)
        {
            __m256 values = _mm256_load_ps(data + i);
            minVector = _mm256_min_ps(minVector, values);
            maxVector = _mm256_max_ps(maxVector, values);
            sumVector = _mm256_add_ps(sumVector, values);
        }

        min = std::min(min, min8(minVector));
        max = std::max(max, max8(maxVector));
        sum += sum8(sumVector);
    }

    for (; i < size; i++)
    {
        float v = data[i];
        min = std::min(v, min);
        max = std::max(v, max);
        sum += v;
    }

    float mean = sum / static_cast<float>(size);
    return {min, max, mean};
}

#endif

std::unordered_map<uint8_t, size_t> computeHistogram(const pybind11::array_t<uint8_t>& image)
{
    if (image.ndim() != 2)
    {
        throw py::value_error("The signal should have two dimensions.");
    }

    std::unordered_map<uint8_t, size_t> histogram;

    auto imageUnchecked = image.unchecked();
    for (py::ssize_t y = 0; y < image.shape(0); y++)
    {
        for (py::ssize_t x = 0; x < image.shape(1); x++)
        {
            histogram[imageUnchecked(y, x)]++;
        }
    }

    return histogram;
}

void initSignalStatisticsPythonModule(py::module& m)
{
    py::class_<SignalStatistics>(m, "SignalStatistics")
        .def(
            py::init<float, float, float>(),
            "Create a new SignalStatistics instance.\n"
            "\n"
            ":param min:\n"
            ":param max:\n"
            ":param mean:\n",
            py::arg("min"),
            py::arg("max"),
            py::arg("mean"))
        .def_property_readonly("min", &SignalStatistics::min, "The minimum value of the signal")
        .def_property_readonly("max", &SignalStatistics::max, "The maximum value of the signal")
        .def_property_readonly("mean", &SignalStatistics::mean, "The mean value of the signal");

    m.def(
        "compute_signal_statistics",
        &computeSignalStatistics,
        "Compute the statistics of the provided signal.\n"
        "\n"
        ":param signal: The 1D signal to process",
        py::arg("signal"));

#if defined(__AVX2__)
    m.def(
        "compute_signal_statistics_simd",
        &computeSignalStatisticsSimd,
        "Compute the statistics of the provided signal.\n"
        "\n"
        ":param signal: The 1D signal to process",
        py::arg("signal"));
#endif

    m.def(
        "compute_histogram",
        &computeHistogram,
        "Compute the histogram of a grayscale image.\n",
        ":param image: The grayscale image",
        py::arg("image"));
}
