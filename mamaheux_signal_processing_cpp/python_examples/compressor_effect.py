import numpy as np
import matplotlib.pyplot as plt

import mamaheux_signal_processing_cpp


def create_signals():
    A_0 = 0.1
    A_1 = 1.0
    F = 100
    T_1 = 0.33
    T_2 = 0.33
    T_3 = 0.33
    FS = 16000

    dt = 1 / FS
    t = np.arange(0, T_1 + T_2 + T_3, dt)

    s1 = A_0 * np.ones(int(T_1 / dt))
    s2 = A_1 * np.ones(int(T_2 / dt))
    s3 = A_0 * np.ones(int(T_3 / dt))

    s_step = np.concatenate((s1, s2, s3))

    if s_step.shape[0] < t.shape[0]:
        s_step = np.concatenate((s_step, np.zeros(t.shape[0] - s_step.shape[0])))

    s_sin = np.sin(2 * np.pi * F * t)

    return t, s_step * s_sin


def main():
    t, input_signal = create_signals()
    output_signal = mamaheux_signal_processing_cpp.apply_compressor(input_signal,
                                                                    threshold=0.4, ratio=0.8,
                                                                    attack=0.99, release=0.9975)

    fig = plt.figure(figsize=(5, 5), dpi=300)
    ax1 = fig.add_subplot(111)

    ax1.plot(t, input_signal, '-', color='tab:blue', label='Input')
    ax1.plot(t, output_signal, '-', color='tab:orange', label='Output')
    ax1.legend()

    plt.show()


if __name__ == "__main__":
    main()
