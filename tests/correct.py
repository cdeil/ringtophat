"""Check ringtophat results against scipy.ndimage.convolve"""
import unittest
from numpy.testing import assert_equal, assert_almost_equal
import numpy as np
import ringtophat

class TestKernels(unittest.TestCase):
    def test_binary_disk(self):
        actual = ringtophat.binary_disk(1)
        desired = np.array([[False, True, False],
                            [ True, True, True],
                            [False, True, False]])
        assert_equal(actual, desired)
    def test_binary_ring(self):
        actual = ringtophat.binary_ring(1, 2)
        desired = np.array([[False, False, True, False, False],
                            [False, True, True, True, False],
                            [ True, True, False, True, True],
                            [False, True, True, True, False],
                            [False, False, True, False, False]])
        assert_equal(actual, desired)

if __name__ == '__main__':
    unittest.main()

