ringtophat
==========

Fast C library for image ring- and tophat-correlation with python bindings

Please give us feedback by filing bug reports and feature requests on github.

Code: https://github.com/gammapy/ringtophat
Issues: https://github.com/gammapy/ringtophat/issues
Documentation: http://ringtophat.readthedocs.org/
Speed: TODO
License: BSD-3-Clause, see LICENSE.txt

Requirements
------------

The C library is C99 without any dependencies.
- It would be nice have utility functions that
  accept ROOT TH2s as input / output.
  Certainly this should be optional!

The python bindings require:
- To be decided: SWIG / Cython?
- To be decided: numpy / scipy?