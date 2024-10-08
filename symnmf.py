import numpy as np
import pandas as pd
import math as maths
import sys
import symnmf as symnmf_mod

np.random.seed(1234)

def get_vars():
    # get the variables from the command line
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

def sym(k, file_name):
    # calculate the similarity matrix
    symnmf_mod.sym(k, file_name)
def ddg(k, file_name):
    # calculate the degree matrix`
    symnmf_mod.ddg(k, file_name)
def norm(k, file_name):
    # calculate the normalized matrix
    symnmf_mod.norm(k, file_name)
def symnmf(k, file_name):
    # calculate the symnmf
    w = norm(k, file_name)
    m = np.mean(w)
    n = w.shape[0]
    h = np.random.uniform(0, 2*maths.sqrt(m/k), (n, k))
    symnmf_mod.symnmf(h, w)


def main():
    # get the variables
    k, calc_type, file_name = get_vars()

    # perform the desired calculation
    if calc_type == "sym":
        sym(k, file_name)
    elif calc_type == "ddg":
        ddg(k, file_name)
    elif calc_type == "norm":
        norm(k, file_name)
    elif calc_type == "symnmf":
        symnmf(k, file_name)
    else:
        print("An Error Has Occurred")
