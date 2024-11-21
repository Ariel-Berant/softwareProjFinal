import numpy as np
import pandas as pd
import math as maths
import sys
import symnmf_c as symnmf_mod

np.random.seed(1234)


def init_vector_list(file_name):
    """
    Load N vectors (data points) from the provided file.

    :param file_name: The path to the input file, the file extension is .txt
    :type file_name: str
    :return: A list of N lists, each list is a vector with d coordinates
    :rtype: list of (list of float)
    """
    data = pd.read_csv(file_name, header=None)
    return data.values.tolist()


def handle_error():
    """
    Print the required error message and terminate the program.

    :return: None
    """
    print("An Error Has Occurred\n")
    sys.exit(1)


def get_vars():
    """
    Get the variables from the command line.

    :return: k - number of clusters
             calc_type - type of calculation to perform
             file_name - name of the file that contains the input vectors
    :rtype k: int
    :rtype calc_type: str
    :rtype file_name: str
    """
    try:
        k = int(sys.argv[1])
    except ValueError:
        handle_error()
    calc_type = sys.argv[2]
    file_name = sys.argv[3]
    try:
        with open(file_name) as f:
            pass
    except FileNotFoundError:
        handle_error()
    return k, calc_type, file_name


def print_matrix(lst):
    """
    Print matrix in the requested form.

    :param lst: matrix of floats to be printed
    :type lst: list of (list of float)
    :return: None
    """
    str_lst = [["%.4f" % elem for elem in row] for row in lst]
    for row in str_lst:
        print(",".join(row))


def sym(file_name):
    """
    Calculate the similarity matrix of the vectors in the given file.

    :param file_name: name of the file that contains the input vectors
    :type file_name: str
    :return: similarity matrix A of vectors in file (size:N*N)
    :rtype: list of (list of float)
    """
    return symnmf_mod.sym(init_vector_list(file_name))


def ddg(file_name):
    """
    Calculate the diagonal degree matrix of the vectors in the given file.

    :param file_name: name of file that contains the input vectors
    :type file_name: str
    :return: diagonal degree matrix D of vectors in file (size: N*N)
    :rtype: list of (list of float)
    """
    return symnmf_mod.ddg(init_vector_list(file_name))


def norm(file_name):
    """
    Calculate the normalized similarity matrix of the vectors in the given file.

    :param file_name: name of file that contains the input vectors
    :type file_name: str
    :return: normalized similarity matrix W of vectors in file (size: N*N)
    :rtype: list of (list of float)
    """
    return symnmf_mod.norm(init_vector_list(file_name))


def symnmf(k, file_name):
    """
    Calculate the symmetric non-negative matrix factorization of the vectors in the given file.

    :param k: number of clusters
    :type k: int
    :param file_name: name of file that contains the input vectors
    :type file_name: str
    :return: symmetric non-negative matrix factorization H of vectors in file (size: N*k)
    :rtype: list of (list of float)
    """
    w = symnmf_mod.norm(init_vector_list(file_name))
    m = np.mean(w)
    n = len(w)
    h = np.random.uniform(0, 2*maths.sqrt(m/k), (n, k))
    return symnmf_mod.symnmf(h.tolist(), w)


def main():
    """
    Perform the desired calculation and print the result.
    """
    # get the variables
    k, calc_type, file_name = get_vars()

    # perform the desired calculation
    if calc_type == "sym":
        print_matrix(sym(file_name))
    elif calc_type == "ddg":
        print_matrix(ddg(file_name))
    elif calc_type == "norm":
        print_matrix(norm(file_name))
    elif calc_type == "symnmf":
        print_matrix(symnmf(k, file_name))
    else:
        print("An Error Has Occurred")


if __name__ == "__main__":
    main()
