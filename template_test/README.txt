# Attempt to build the library
g++ -fPIC -c myfunction.cc
g++ -shared -o libmyfunction.so myfunction.o
nm libmyfunction.so


# Imports
from scipy.ndimage import convolve
import numpy as np

# Speed test
image = np.random.rand(1000,400)
image = np.random.rand(10000,400)
%timeit convolve(image, kernel)

# Border test
image = np.ones((1000,1000))
kernel = np.ones((10,10))
result = convolve(image, kernel)
result = convolve(image, kernel, mode='constant')
pyfits.writeto('result.fits', result, overwrite=True)
