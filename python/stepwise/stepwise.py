import argparse
import logging
import sys
import warnings

import numpy as np
from scipy.linalg import solve_triangular

# Numpy dtype warning:
# https://github.com/ContinuumIO/anaconda-issues/issues/6678
warnings.filterwarnings("ignore", message="numpy.dtype size changed")
warnings.filterwarnings("ignore", message="numpy.ufunc size changed")


formatter = logging.Formatter('%(asctime)s - [%(levelname)s] - '
                              '{%(name)s:%(funcName)s:%(lineno)s} - '
                              '%(message)s')
ch = logging.StreamHandler(sys.stdout)
ch.setFormatter(formatter)
logger = logging.getLogger()
logger.addHandler(ch)
logger.setLevel(logging.INFO)


class ForwardStepwiseRegression(object):
    def __init__(self, xx, xy, max_iterations, output, **kwargs):
        self._xx = xx
        self._xy = xy
        self._output = output
        self._max_iter = max_iterations

        self._logger = logging.getLogger(
            name=self.__class__.__module__ + "." + self.__class__.__name__)
        if kwargs.pop("verbose"):
            self._logger.setLevel(logging.DEBUG)

    def fit(self):
        # Using AIC Criteria to Select Models
        # Since, in each iteration, all models have same number of features,
        # minimizing AIC comes down to finding to the minimum SSE

        num_features = len(self._xy)
        all_indices = range(num_features)
        remaining_idx = all_indices
        selected_idxs = []
        sse = []
        betas = np.zeros((self._max_iter, num_features))
        num_iter = 0
        while len(remaining_idx) > 0 and num_iter < self._max_iter:
            iter_betas = []
            iter_sse = []
            for feature in remaining_idx:
                check_idx = np.append(selected_idxs, [feature]).tolist()
                self._logger.debug("Running on : {}".format(check_idx))
                b, s = self.subset_regression(self._xx, self._xy, check_idx)
                iter_betas.append(b)
                iter_sse.append(s)

            min_sse_idx = np.argmin(iter_sse)
            idx = remaining_idx[min_sse_idx]

            min_sse = np.min(iter_sse)
            sse.append(min_sse)
            self._logger.debug("Iteration Betas: {}".format(iter_betas))
            self._logger.debug("Iteration Min SSE: {}".format(min_sse))

            selected_idxs.append(idx)
            self._logger.debug("Selected Indices: {}".format(selected_idxs))
            remaining_idx = np.setdiff1d(all_indices, selected_idxs)
            self._logger.debug("Remaining Indices: ".format(remaining_idx))
            betas[num_iter, selected_idxs] = iter_betas[min_sse_idx]
            num_iter += 1

        np.savetxt(self._output, betas, delimiter=",", fmt="%1.10f")

    @staticmethod
    def subset_regression(xx_in, xy_in, indices):
        xx_filtered = np.take(np.take(xx_in, indices, axis=1), indices, axis=0)
        xy_filtered = np.take(xy_in, indices)
        R = np.linalg.cholesky(xx_filtered)
        w = solve_triangular(a=R, b=xy_filtered, lower=True)
        betas = solve_triangular(R.T, w)
        sse = - betas.T.dot(xy_filtered) # Reduced SSE: we do not know y_T * y
        return betas, sse


if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument("--xx", type=str, required=True,
                        help="x_transpose * x input filename")
    parser.add_argument("--xy", type=str, required=True,
                        help="x_transpose * y input filename")
    parser.add_argument("--max_iterations", type=int, required=True,
                        help="output filename")
    parser.add_argument("--output", type=str, required=True,
                        help="output filename")
    parser.add_argument("--verbose", action='store_true', default=False,
                        help="Verbose Output")

    args = parser.parse_args()

    kw = {"verbose": args.verbose}

    xx = np.loadtxt(args.xx, delimiter=",")
    xy = np.loadtxt(args.xy, delimiter=",")

    reg = ForwardStepwiseRegression(xx, xy, args.max_iterations,
                                    args.output, **kw)
    reg.fit()
