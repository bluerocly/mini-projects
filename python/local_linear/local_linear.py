import argparse
import logging
import sys

import numpy as np
import matplotlib.pyplot as plt

formatter = logging.Formatter('%(asctime)s - [%(levelname)s] - '
                              '{%(name)s:%(funcName)s:%(lineno)s} - '
                              '%(message)s')
ch = logging.StreamHandler(sys.stdout)
ch.setFormatter(formatter)
logger = logging.getLogger()
logger.addHandler(ch)
logger.setLevel(logging.INFO)


class LocalLinearRegression(object):
    def __init__(self, xin, yin, output, num_folds, **kwargs):
        self._xin = xin
        self._yin = yin
        self._output = output
        self._folds = num_folds

        self._xout = kwargs.pop("xout", xin)
        self._plot = kwargs.pop("plot", False)
        self._logger = logging.getLogger(
            name=self.__class__.__module__ + "." + self.__class__.__name__)
        if kwargs.pop("verbose"):
            self._logger.setLevel(logging.DEBUG)

    def fit(self):
        x_splits, y_splits = self._split_k_fold(
            self._xin, self._yin, self._folds)
        cv_bw = self._cross_validate(x_splits, y_splits)
        yout = self.estimate(self._xout, self._xin, self._yin, cv_bw)
        np.savetxt(self._output, yout, fmt='%f')

        if self._plot:
            plt.figure()
            sorted_idx = np.argsort(self._xout)
            plt.title(
                "Local Linear Regression Fit: Gaussian Kernel (h = {})".format(
                    cv_bw))
            plt.scatter(x=self._xin, y=self._yin, alpha=0.2, color='b', s=200)
            plt.plot(self._xout[sorted_idx], yout[sorted_idx], color='r',
                     linewidth=5)
            plt.show()

    def estimate(self, x_test, x_train, y_train, bw):
        if len(x_train) != len(y_train):
            raise ValueError("X and Y Should have same length")
        B = np.array([np.ones(len(x_train)), x_train]).T
        y_hat = []
        for x0 in x_test:
            W = np.diag(self._gaussian_kernel(x_train, x0, bw))
            y_hat.append(np.array([1, x0]).T.dot(
                np.linalg.inv(B.T.dot(W).dot(B))).dot(
                B.T).dot(W).dot(y_train))

        return np.array(y_hat)

    def _cross_validate(self, x_splits, y_splits):
        fold_indices = np.arange(self._folds)
        # Hard Coded - # No instructions regarding this.
        # General Range : [0.1, 2.0] Unstable Outside this range
        h_values = np.linspace(0.1, 2.0, 20)
        mse_values = []

        for h in h_values:
            self._logger.debug("Fitting for h = {}".format(h))
            mses = []
            for idx in fold_indices:
                test_idx = idx
                train_idx = np.setdiff1d(fold_indices, [idx])
                train_x, test_x, train_y, test_y = (
                    np.concatenate(x_splits[train_idx]),
                    x_splits[test_idx],
                    np.concatenate(y_splits[train_idx]),
                    y_splits[test_idx])
                test_y_hat = self.estimate(test_x, train_x, train_y, h)
                mses.append(np.mean(np.power(test_y_hat - test_y, 2)))
            mse_values.append(np.mean(mses))

        fitted_bw = h_values[np.argmin(mse_values)]

        self._logger.info("Cross Validated Bandwidth: {}".format(fitted_bw))

        return fitted_bw

    def _gaussian_kernel(self, x, x0, h):
        return np.exp(- 0.5 * np.power((x - x0) / h, 2))

    @staticmethod
    def _split_k_fold(x, y, folds):
        if len(x) != len(y):
            raise ValueError("X and Y Should have same length")
        indices = np.arange(len(x))
        np.random.shuffle(indices)
        split_size = len(x) / folds
        return np.array(
            [x[n * split_size:(n + 1) * split_size]
             for n in np.arange(folds)]), np.array(
            [y[n * split_size:(n + 1) * split_size]
             for n in np.arange(folds)])


if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument("--xin", type=str, required=True,
                        help="x input filename")
    parser.add_argument("--yin", type=str, required=True,
                        help="y input filename")
    parser.add_argument("--output", type=str, required=True,
                        help="output filename")
    parser.add_argument("--num_folds", type=int, required=True,
                        help="output filename")
    parser.add_argument("--xout", type=str, required=False, default=None,
                        help="output filename")
    parser.add_argument("--plot", action='store_true', default=False,
                        help="output filename")
    parser.add_argument("--verbose", action='store_true', default=False,
                        help="Verbose Output")

    args = parser.parse_args()

    kw = {"plot": args.plot, "verbose": args.verbose}

    if args.xout:
        kw["xout"] = np.loadtxt(args.xout)

    ll_reg = LocalLinearRegression(np.loadtxt(args.xin), np.loadtxt(args.yin),
                                   args.output, args.num_folds, **kw)
    ll_reg.fit()
