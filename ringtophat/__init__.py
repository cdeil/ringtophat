"""ringtophat --- Fast C library for image ring- and
tophat-correlation with python bindings"""
import numpy as np
import logging
import scipy.ndimage

DEFAULT_MODE = 'constant'


def _get_structure_indices(radius):
    """
    Get arrays of indices for a symmetric structure,
    i.e. with an odd number of pixels and 0 at the center
    """
    radius = int(radius)
    y, x = np.mgrid[-radius:radius + 1, -radius:radius + 1]
    return x, y


def binary_disk(radius):
    """
    Generate a binary disk.
    Value 1 inside and 0 outside.

    Useful as a structure element for morphological transformations.

    Note that the returned structure always has an odd number
    of pixels so that shifts during correlation are avoided.
    """
    x, y = _get_structure_indices(radius)
    structure = x ** 2 + y ** 2 <= radius ** 2
    return structure


def binary_ring(r_in, r_out):
    """
    Generate a binary ring.
    Value 1 inside and 0 outside.

    Useful as a structure element for morphological transformations.

    Note that the returned structure always has an odd number
    of pixels so that shifts during correlation are avoided.
    """
    x, y = _get_structure_indices(r_out)
    mask1 = r_in ** 2 <= x ** 2 + y ** 2
    mask2 = x ** 2 + y ** 2 <= r_out ** 2
    return mask1 & mask2


def tophat_correlate(data, radius, mode=DEFAULT_MODE):
    """ Correlate with disk of given radius """
    logging.debug('tophat_correlate shape = {0}, radius = {1}'
                  ''.format(data.shape, radius))
    structure = binary_disk(radius)
    return scipy.ndimage.convolve(data, structure, mode=mode)


def tophat_pixel_correction(radius):
    """ Return area correction factor due to pixelation """
    actual = binary_disk(radius).sum()
    desired = np.pi * radius ** 2
    pixel_correction = actual / desired
    logging.debug('pixel_correction = {0}'.format(pixel_correction))
    return pixel_correction


def ring_correlate(data, r_in, r_out, mode=DEFAULT_MODE):
    """ Correlate with ring of given radii """
    logging.debug('ring_correlate shape = {0}, r_in = {1}, r_out = {2}'
                  ''.format(data.shape, r_in, r_out))
    structure = binary_ring(r_in, r_out)
    return scipy.ndimage.convolve(data, structure, mode=mode)


def ring_pixel_correction(r_in, r_out):
    """ Return area correction factor due to pixelation """
    actual = binary_ring(r_in, r_out).sum()
    desired = np.pi * (r_out ** 2 - r_in ** 2)
    pixel_correction = actual / desired
    logging.debug('pixel_correction = {0}'.format(pixel_correction))
    return pixel_correction


def example_image():
    """Make an example image for tests"""
    image = np.zeros((100, 200))
    image[0, 0:10] = 1
    image[50, 30] = 2
    image[-1, -1] = 1
    image[50, 0] = 1
    return image
