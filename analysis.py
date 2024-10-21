import numpy as np
# import pandas as pd
# import math as maths
import sys
import symnmf as symnmf_py
from sklearn.metrics import silhouette_score


def get_vars():
    try:
        k = int(sys.argv[1])
    except ValueError:
        print("An Error Has Occurred")
        sys.exit(1)
    file_name = sys.argv[2]
    return k, file_name


def cluster_assignment(h):
    rows, cols = h.shape[0], h.shape[1]
    labels = np.zeros(rows)
    for row in range(rows):
        max_col = h[row][0]
        label = 0
        for col in range(cols):
            if h[row][col] >= max_col:
                max_col = h[row][col]
                label = col
        labels[row] = label
    return labels


def main():
    k, file_name = get_vars()
    h = symnmf_py.symnmf(k, file_name)
    labels = cluster_assignment(h)
    print("nmf: ", silhouette_score(h, labels))


if __name__ == "__main__":
    main()
