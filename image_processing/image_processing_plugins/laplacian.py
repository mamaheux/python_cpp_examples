import numpy as np
import cv2


def name() -> str:
    return "Laplacian"


def process(bgr_image: np.ndarray) -> np.ndarray:
    gray_image = cv2.cvtColor(bgr_image, cv2.COLOR_BGR2GRAY)
    return cv2.cvtColor(cv2.Laplacian(gray_image, -1, ksize=1), cv2.COLOR_GRAY2BGR)


