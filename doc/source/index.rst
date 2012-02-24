.. ringtophat documentation master file, created by
   sphinx-quickstart on Fri Feb 24 17:10:25 2012.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

ringtophat
==========

Contents:

.. toctree::
   :maxdepth: 2


Fast C library for image ring- and tophat-correlation with python bindings


Please give us feedback by filing bug reports and feature requests on github.


* Code: https://github.com/gammapy/ringtophat
* Issues: https://github.com/gammapy/ringtophat/issues
* Documentation: http://ringtophat.readthedocs.org/
* Speed: TODO
* License: BSD-3-Clause, see LICENSE.txt

Requirements
------------

The C library is C99 without any dependencies.

* It would be nice have utility functions that accept ROOT TH2s as input / output. Certainly this should be optional!


The python bindings require:

* To be decided: SWIG / Cython?
* To be decided: numpy / scipy?

What is ring- and tophat-correlation?
-------------------------------------

* @todo Show image.

Aren't hard-edge filters bad? Where is this useful?
---------------------------------------------------

* E.g. gamma-ray and X-ray astronomy.
* @todo Explain ring background method.
* @todo Give references to Berge and X-ray papers.

Why not simply use a general correlation method from an existing library?
-------------------------------------------------------------------------

* @todo Speed. Illustrate with image.


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

