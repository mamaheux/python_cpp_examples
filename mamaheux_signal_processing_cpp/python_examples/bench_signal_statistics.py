import sys
sys.path.append('../cmake-build-release')

import numpy as np
import time

import mamaheux_signal_processing_cpp


def compute_signal_statistics_python(signal):
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
    
    return min_value, max_value, mean_value
    
def compute_signal_statistics_numpy(signal):
    if len(signal.shape) != 1:
        raise ValueError('The signal should have only one dimension.')

    min_value = np.min(signal)
    max_value = np.max(signal)
    mean_value = np.mean(signal)
    
    return min_value, max_value, mean_value


def compute_signal_statistics_cpp(signal):
    stats = mamaheux_signal_processing_cpp.compute_signal_statistics(signal)
    return stats.min, stats.max, stats.mean



def compute_signal_statistics_cpp_simd(signal):
    stats = mamaheux_signal_processing_cpp.compute_signal_statistics_simd(signal)
    return stats.min, stats.max, stats.mean



def bench(signal, function, name):
    start_time = time.time()
    min_value, max_value, mean_value = function(signal)
    end_time = time.time()
    
    print(name)
    print(f'{min_value=}, {max_value=}, {mean_value=}')
    print(f'Duration: {end_time-start_time} s')
    print()


def main():
    N = 100000000
    signal = np.random.rand(N).astype(np.float32)
    
    bench(signal, compute_signal_statistics_python, 'Python')
    bench(signal, compute_signal_statistics_numpy, 'Numpy')
    bench(signal, compute_signal_statistics_cpp, 'C++')
    bench(signal, compute_signal_statistics_cpp_simd, 'C++ SIMD')
    

if __name__ == "__main__":
    main()
        
    