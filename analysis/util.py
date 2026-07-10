import numpy as np

def moving_average_5_with_edges(arr):
    arr = np.asarray(arr, dtype=float)
    n = len(arr)
    smoothed = np.empty(n, dtype=float)

    for i in range(n):
        start = max(0, i - 2)
        end = min(n, i + 3)
        smoothed[i] = np.mean(arr[start:end])

    return smoothed

def interpolate_crossings(distances, smoothed_intensity, target):
    crossings = []
    for i in range(len(smoothed_intensity) - 1):
        y0, y1 = smoothed_intensity[i], smoothed_intensity[i + 1]

        # exact hit
        if y0 == target:
            crossings.append(float(distances[i]))

        # negative value indicates that target is between y0 and y1, making them sufficient for interpolation
        elif (y0 - target) * (y1 - target) < 0: 
            x0, x1 = distances[i], distances[i + 1]
            x_cross = x0 + (target - y0) * (x1 - x0) / (y1 - y0) # linear interpolation
            crossings.append(float(x_cross))

    return crossings
