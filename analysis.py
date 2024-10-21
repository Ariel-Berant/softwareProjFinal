import numpy as np
# import pandas as pd
import math as maths
import sys
import symnmf as symnmf_mod
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
    rows, cols = len(h), len(h[0])
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




# code from hw1, rewritten to use the functions above and for better readability

def euclidian_distance_calc(vect1, vect2):
    diff_sum = 0
    for i in range(len(vect1)):
        diff_sum += (vect1[i] - vect2[i]) ** 2
    return maths.sqrt(diff_sum)


def is_converged(vects_arr1, vects_arr2, epsilon):
    conv_sum = 0
    for i in range(len(vects_arr1)):
        if euclidian_distance_calc(vects_arr1[i], vects_arr2[i]) < epsilon:
            conv_sum += 1
    return conv_sum == len(vects_arr1)


def calc_centroids(clusters):
    new_centroids = [[] for i in range(len(clusters))]

    for j in range(len(clusters)):
        for i in range(len(clusters[j][0])):
            curr_sum = 0

            # calculate sum of ith coordinate
            for vect in clusters[j]:
                curr_sum += vect[i]

            # add sum of ith coordinates divided by cluster sizes
            new_centroids[j].append(curr_sum / len(clusters[j]))

    return new_centroids


def get_vects_from_file(input_data):

    # check file is ok and init vectors
    if not input_data.endswith('.txt'):
        exit(1)
    file = open(input_data, "r")
    input_vectors = []

    # read file line by line and add vectors to list
    curr_line = file.readline()
    while curr_line != "":
        input_vectors.append([float(coord) for coord in curr_line.split(',')])
        curr_line = file.readline()
    file.close()
    return input_vectors


def kmeans(k, input_data, iters):
    # get vectors from file
    input_vectors = get_vects_from_file(input_data)

    # initialize variables
    centroids = input_vectors[:k]
    epsilon = 0.001

    # initialize centroids
    for i in range(k):
        centroids.append(input_vectors[i])

    for i in range(iters):
        cluster_list = [[] for i in range(k)]

        for vect in input_vectors:
            # calculate distance to all centroids
            centroids_dist = [euclidian_distance_calc(vect, centroid) for centroid in centroids]
            curr_min_dist = centroids_dist[0]
            curr_min_ind = 0

            # find index of closest centroid and add to closest cluster
            for j in range(1, len(centroids_dist)):
                if centroids_dist[j] < curr_min_dist:
                    curr_min_dist = centroids_dist[j]
                    curr_min_ind = j
            cluster_list[curr_min_ind].append(vect)

        # calculate new centroids, distance from last and update
        new_centroids = calc_centroids(cluster_list)
        if is_converged(new_centroids, centroids, epsilon):
            return new_centroids
        centroids = new_centroids

    return centroids


def symnmf(k, file_name):
    # calculate the symnmf
    w = symnmf_mod.norm(file_name)
    m = np.mean(w)
    n = len(w)
    h = np.random.uniform(0, 2*maths.sqrt(m/k), (n, k))
    return symnmf_mod.symnmf(h.tolist(), w)


def main():
    k, file_name = get_vars()
    h = symnmf(k, file_name)
    labels = cluster_assignment(h)
    print("nmf: ", silhouette_score(h, labels))


if __name__ == "__main__":
    main()
