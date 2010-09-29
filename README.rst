SLHAea - another SUSY Les Houches Accord input/output library
=============================================================

Introduction
------------

SLHAea is an easy to use C++ library for input, output, and
manipulation of data in the `SUSY Les Houches Accord`_ (SLHA). It is
based on the concept that a SLHA structure is a container of blocks,
which are then again containers of lines, which are then again
containers of strings.

Its main features are:

- reading and writing SLHA files
- easy access to individual blocks, lines, and fields
- three containers (``Coll``, ``Block``, and ``Line``) that mimic STL
  containers and therefore offer great flexibility
- the exact formatting of lines is preserved
- everything is stored as strings, so data is not restricted to floats
  or integers
- no precision is lost in read/write cycles of unmodified data
- blocks and lines are not restricted to the ones specified in the
  `SLHA`_, `SLHA2`_ and `FLHA`_ or later accords that use the same
  structure

.. _SUSY Les Houches Accord: http://home.fnal.gov/~skands/slha/
.. _SLHA:  http://arxiv.org/abs/hep-ph/0311123
.. _SLHA2: http://arxiv.org/abs/0801.0045
.. _FLHA:  http://arxiv.org/abs/1008.0762

Documentation
-------------

The API documentation is `here`_. It includes some `practical
examples`_ which can also be found in the `doc/examples/`_ directory
in SLHAea's source tree.

.. _here: http://www.physik.uni-wuerzburg.de/~fthomas/slhaea/doc/
.. _practical examples:
    http://www.physik.uni-wuerzburg.de/~fthomas/slhaea/doc/examples.html
.. _doc/examples/: http://github.com/fthomas/slhaea/tree/master/doc/examples/

Dependencies
------------

To use SLHAea only the C++ Standard Library and some headers from the
`Boost C++ Libraries`_ are required.

.. _Boost C++ Libraries: http://www.boost.org/

Download
--------

You can download SLHAea in either `tar.gz`_ or `zip`_ formats.

.. _tar.gz: http://github.com/fthomas/slhaea/tarball/master
.. _zip: http://github.com/fthomas/slhaea/zipball/master

The version control system used for development of SLHAea is Git. The
`Git repository`_ can be inspected and browsed online at GitHub and it
can be cloned by running::

  git clone git://github.com/fthomas/slhaea.git

.. _Git repository: http://github.com/fthomas/slhaea

Issues and feedback
-------------------

For bug reports, feature requests, or general feedback either use the
`issue tracker`_ or `write me an email`_.

.. _issue tracker: http://github.com/fthomas/slhaea/issues
.. _write me an email: fthomas@physik.uni-wuerzburg.de

License
-------

SLHAea is `free software`_ and licensed under the `Boost Software
License 1.0`_. The full text of the license can be found in the file
``LICENSE_1_0.txt`` in SLHAea's source tree.

.. _free software: http://www.gnu.org/philosophy/free-sw.html
.. _Boost Software License 1.0:  http://www.boost.org/users/license.html

Author
------

SLHAea was written by
Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>.
