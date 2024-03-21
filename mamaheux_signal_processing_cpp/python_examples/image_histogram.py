import numpy as np
import matplotlib.pyplot as plt

import mamaheux_signal_processing_cpp


def main():
    image = np.random.randint(0, 255, size=(256, 128), dtype=np.uint8)
    histogram = mamaheux_signal_processing_cpp.compute_histogram(image)

    values = list(histogram.keys())
    counts = list(histogram.values())

    fig = plt.figure(figsize=(6, 6), dpi=200)
    ax = fig.add_subplot(111)
    ax.bar(values, counts)
    plt.show()


if __name__ == "__main__":
    main()
