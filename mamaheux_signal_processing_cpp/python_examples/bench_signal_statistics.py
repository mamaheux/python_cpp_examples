import time
from typing import List, Callable, Tuple

import numpy as np
from numba import njit
import matplotlib.pyplot as plt

import mamaheux_signal_processing_cpp


def compute_signal_statistics_python(signal: np.ndarray) -> mamaheux_signal_processing_cpp.SignalStatistics:
    if len(signal.shape) != 1:
        raise ValueError('The signal should have only one dimension.')

    min_value = float('inf')
    max_value = float('-inf')
    mean_value = 0.0

    for v in signal:
        if v < min_value:
            min_value = v
        if v > max_value:
            max_value = v

        mean_value += v

    mean_value /= signal.shape[0]

    return mamaheux_signal_processing_cpp.SignalStatistics(min_value, max_value, mean_value)


def compute_signal_statistics_numpy(signal: np.ndarray) -> mamaheux_signal_processing_cpp.SignalStatistics:
    if len(signal.shape) != 1:
        raise ValueError('The signal should have only one dimension.')

    min_value = np.min(signal)
    max_value = np.max(signal)
    mean_value = np.mean(signal)

    return mamaheux_signal_processing_cpp.SignalStatistics(min_value, max_value, mean_value)


def compute_signal_statistics_numba(signal: np.ndarray) -> mamaheux_signal_processing_cpp.SignalStatistics:
    if len(signal.shape) != 1:
        raise ValueError('The signal should have only one dimension.')

    min_value, max_value, mean_value = _compute_signal_statistics_numba_internal(signal)
    return mamaheux_signal_processing_cpp.SignalStatistics(min_value, max_value, mean_value)


@njit
def _compute_signal_statistics_numba_internal(signal: np.ndarray) -> Tuple[float, float, float]:
    type_info = np.finfo(signal.dtype)
    min_value = type_info.max
    max_value = type_info.min
    mean_value = 0.0

    for v in signal:
        if v < min_value:
            min_value = v
        if v > max_value:
            max_value = v

        mean_value += v

    mean_value /= signal.shape[0]

    return min_value, max_value, mean_value


def bench(signals: np.ndarray, function: Callable) -> List[float]:
    if len(signals) > 0:
        function(signals[0]) # Warm up

    N = 100
    durations = []

    for signal in signals:
        start_time = time.time()
        for c in range(1, N + 1):
            _stats = function(signal)
        durations.append((time.time() - start_time) / N)

    return durations


def main():
    fig = plt.figure(figsize=(6, 6), dpi=200)
    ax = fig.add_subplot(111)

    sizes = np.linspace(100, 1000000, 100, dtype=int)
    signals = [np.random.rand(size).astype(np.float32) for size in sizes]

    python_count = 10
    duration_python = bench(signals[:python_count], compute_signal_statistics_python)
    ax.plot(sizes[:python_count], duration_python, '-', label='Python')

    duration_numpy = bench(signals, compute_signal_statistics_numpy)
    ax.plot(sizes, duration_numpy, '-', label='Numpy')

    duration_numba = bench(signals, compute_signal_statistics_numba)
    ax.plot(sizes, duration_numba, '-', label='Numba')

    duration_cpp = bench(signals, mamaheux_signal_processing_cpp.compute_signal_statistics)
    ax.plot(sizes, duration_cpp, '-', label='C++')
    if hasattr(mamaheux_signal_processing_cpp, 'compute_signal_statistics_simd'):
        duration_cpp_simd = bench(signals, mamaheux_signal_processing_cpp.compute_signal_statistics_simd)
        ax.plot(sizes, duration_cpp_simd, '-', label='C++ SIMD')

    ax.legend()
    ax.set_yscale('log')
    ax.set_xlabel('Signal Size')
    ax.set_ylabel('Duration (s)')
    ax.grid(True, which='both')
    plt.show()


if __name__ == "__main__":
    main()

