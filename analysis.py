import numpy as np
import sys
import symnmf as symnmf_mod
import kmeans as kmeans_py
from sklearn.metrics import silhouette_score

np.random.seed(1234)


def get_vars():
    """
    Get the variables from the command line.

    :return: k - number of clusters
             file_name - name of the file that contains the input vectors
    :rtype k: int
    :rtype file_name: str
    """
    try:
        k = int(sys.argv[1])
    except ValueError:
        symnmf_mod.handle_error()
    file_name = sys.argv[2]
    return k, file_name


def cluster_assignment(h):
    """
    Assign clusters according to the symNMF algorithm.

    :param h: an association matrix (resulting from the SymNMF algorithm in our case) (size: N*k)
    :type h: list of (list of float)
    :return: a vector where the ith entry is the cluster number of the ith row (vector) (size: N)
    :rtype: np.ndarray
    """
    return np.array(h).argmax(axis=1)


def main():
    """
    Compare SymNMF to Kmeans using silhouette score.
    """
    # get the variables and calc h and labels
    k, file_name = get_vars()
    h = symnmf_mod.symnmf(k, file_name)
    labels = cluster_assignment(h)
    data = symnmf_mod.init_vector_list(file_name)
    
    # calc and print silhouette score for symNMF
    print("nmf: %.4f" % silhouette_score(data, labels))
    
    # calc labels for kmeans
    labels = kmeans_py.find_closest_centroids(data, kmeans_py.kmeans(k, file_name, 100))
    
    # calc and print silhouette score for kmeans
    print("kmeans: %.4f" % silhouette_score(data, labels))


if __name__ == "__main__":
    main()
