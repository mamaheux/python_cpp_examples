import numpy as np
import cv2


def name() -> str:
    return "Blur"


def process(bgr_image: np.ndarray) -> np.ndarray:
    return cv2.blur(bgr_image, (5, 5))
