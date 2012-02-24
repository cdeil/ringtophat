#!/usr/bin/env python
from distutils.core import setup, Extension

setup(name="ringtophat",
      version="0.1",
      description="Fast C library for image ring- and tophat-correlation with python bindings",
      author="Ramin Marx, Christoph Deil",
      author_email="ringtophat@gmail.com",
      url="https://github.com/gammapy/ringtophat",
      # @todo: C extension stuff
      # https://github.com/gammapy/ringtophat/issues/1
      )