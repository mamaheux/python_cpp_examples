import numpy as np
import matplotlib.pyplot as plt

import mamaheux_signal_processing_cpp


def main():
    image = np.random.randint(0, 255, size=(256, 128), dtype=np.uint8)
    histogram = mamaheux_signal_processing_cpp.compute_histogram(image)

    values = list(histogram.keys())
    counts = list(histogram.values())

    fig = plt.figure(figsize=(5, 5), dpi=300)
    ax1 = fig.add_subplot(111)

    ax1.bar(values, counts)
    ax1.legend()

    plt.show()


if __name__ == "__main__":
    main()
