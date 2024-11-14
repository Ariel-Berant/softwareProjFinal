import math as maths

# code from hw1, rewritten to use the functions above and for better readability


def euclidian_distance_calc(vect1, vect2):
    """
    Calculate the Euclidian distance between two vectors.

    :param vect1: the first vector
    :type vect1: list of float
    :param vect2: the second vector
    :type vect2: list of float
    :return: the Euclidian distance between vect1 and vect2
    :rtype: float
    """
    diff_sum = 0
    for i in range(len(vect1)):
        diff_sum += (vect1[i] - vect2[i]) ** 2
    return maths.sqrt(diff_sum)


def is_converged(vects_arr1, vects_arr2, epsilon):
    """
    Check if two vectors have converged (i.e. the Euclidian distance between them is less than epsilon)

    :param vects_arr1: first vector
    :type vects_arr1: list of (list of float)
    :param vects_arr2: second vector
    :type vects_arr2: list of (list of float)
    :param epsilon: threshold for distance between vectors for convergence
    :type epsilon: float
    :return: True if converged, False otherwise
    :rtype: bool
    """
    conv_sum = 0
    for i in range(len(vects_arr1)):
        if euclidian_distance_calc(vects_arr1[i], vects_arr2[i]) < epsilon:
            conv_sum += 1
    return conv_sum == len(vects_arr1)


def calc_centroids(clusters):
    """
    Calculate centroids given a list of clusters.

    :param clusters: clusters formed by the kmeans algorithm
    :type clusters: list of (list of (list of float))
    :return: centroids of the clusters
    :rtype: list of (list of float)
    """
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
    """
    Retrieves the input vectors from a given .txt file.

    :param input_data: .txt file path with input vector data on it
    :type input_data: str
    :return: a matrix compose of the input vectors
    :rtype: list of (list of float)
    """

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
    """
    Run the kmeans algorithm on the given data.

    :param k: number of clusters
    :type k: int
    :param input_data: .txt file path with input vector data on it
    :type input_data: str
    :param iters: max number of iterations of the algorithm
    :type iters: int
    :return: centroids that result from running the kmeans algorithm on the input data
    :rtype: list of (list of float)
    """
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

            # find index of closest centroid and add to the closest cluster
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


def find_closest_centroids(vectors, centroids):
    """
    Find the closest centroid to each vector from input list.

    :param vectors: input vectors
    :type vectors: list of (list of float)
    :param centroids: centroids of existing clusters
    :type centroids: list of (list of float)
    :return: a vector where the ith entry is the centroids closest to the ith vector
    :rtype: list of int
    """
    # init empty list
    labels = []

    # calculate distance to all centroids for every vector, and label is min distance
    for i in range(len(vectors)):
        centroids_dist = [euclidian_distance_calc(vectors[i], centroid) for centroid in centroids]
        labels.append(centroids_dist.index(min(centroids_dist)))

    return labels
