import numpy as np
import pandas as pd
import math as maths
import sys
import symnmf as symnmf_mod

np.random.seed(1234)


def get_vars():
    # get the variables from the command line
    # does "No need to validate arguments" include checking the amount of args? thr type? etc
    try:
        k = int(sys.argv[1])
    except ValueError:
        print("An Error Has Occurred")
        sys.exit(1)
    calc_type = sys.argv[2]
    file_name = sys.argv[3]
    return k, calc_type, file_name


def load_vects(file_name):
    # load the vectors from the file
    data = pd.read_csv(file_name, header=None)
    return data


def print_matrix(lst):
    # turn the list of lists into a string and print it
    str_lst = [["%.4f" % elem for elem in row] for row in lst]
    for row in str_lst:
        print(",".join(row))
    print("")


def sym(file_name):
    # calculate the similarity matrix
    print_matrix(symnmf_mod.sym(file_name))


def ddg(file_name):
    # calculate the degree matrix
    print_matrix(symnmf_mod.ddg(file_name))


def norm(file_name):
    # calculate the normalized matrix
    return symnmf_mod.norm(file_name)


def symnmf(k, file_name):
    # calculate the symnmf
    w = symnmf_mod.norm(file_name)
    m = np.mean(w)
    n = len(w)
    h = np.random.uniform(0, 2*maths.sqrt(m/k), (n, k))
    return symnmf_mod.symnmf(h.tolist(), w)


def main():
    # get the variables
    k, calc_type, file_name = get_vars()

    # perform the desired calculation
    if calc_type == "sym":
        sym(file_name)
    elif calc_type == "ddg":
        ddg(file_name)
    elif calc_type == "norm":
        print_matrix(norm(file_name))
    elif calc_type == "symnmf":
        print_matrix(symnmf(k, file_name))
    else:
        print("An Error Has Occurred")


if __name__ == "__main__":
    main()
