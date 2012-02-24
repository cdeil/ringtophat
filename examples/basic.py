"""Basic usage examples for ringtophat"""
import numpy as np
import ringtophat

if __name__ == '__main__':
    image = ringtophat.example_image()
    top_image = ringtophat.tophat_correlate(image, radius=10)
    ring_image = ringtophat.ring_correlate(image, r_i=10, r_o=20)