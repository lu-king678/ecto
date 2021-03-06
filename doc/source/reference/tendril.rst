.. _tendril-overview:

tendril
=======

.. _boost_any: http://www.boost.org/doc/libs/1_47_0/doc/html/any.html

The basic way of communication for a cell (whether as an input/output or as a parameter)
is through a C++ object called ``tendril``. A tendril abstracts the C++ type of the object
it contains to allow the scheduling to be generic, to allow static introspection and to
ease the Python interaction.

It is behaving like a `boost::any <boost_any>`_ (think `void*` with an encoded type) augmented
with certain conversion rules, operators, introspection capabilities,
docstrings and the like.


Overview
--------

You will primarily encounter the ``tendril`` as the right hand side of
the mappings which are the ``tendrils`` objects passed to ecto cell
functions, that is, a tendrils object is essentially a map of
``std::string`` -> ``shared_ptr<tendril>``.  Each ecto cell has
associated with it three sets of tendrils: parameters, input, and
output.  The *scheduler*, external to the cell, copies data between
one cell's output tendril and the input tendril of another cell.

.. literalinclude:: ../src/Example01.cpp
   :language: cpp
   :start-after: using
   :end-before: ECTO_CELL

See :download:`../src/Example01.cpp`

A script like this,

.. literalinclude:: ../src/Example01.py
   :language: py

Will output,

.. program-output:: ../src/Example01.py
   :prompt:
   :in_srcdir:

.. _tendril-conversions:

Tendril Conversions
^^^^^^^^^^^^^^^^^^^

The table below explains (somewhat) the type conversions that happen
when one tendril or type is inserted to another.

.. rubric:: Tendril Conversions

+------------+---------------------+----------------+------------+------------+
|        FROM|                     |                |            |            |
|            |                     |python          |            |            |
|TO          |``none``             |object          |``T``       |``U``       |
+------------+---------------------+----------------+------------+------------+
|``none``    |assignment (no-op)   |python          |``T``       |``U``       |
|            |                     |object          |            |            |
|            |                     |                |            |            |
+------------+---------------------+----------------+------------+------------+
|python      |ValueNone error      |assignment      |python      |python      |
|object      |                     |                |object      |object      |
|            |                     |                |            |            |
|            |                     |                |            |            |
+------------+---------------------+----------------+------------+------------+
|``T``       |ValueNone error      |``T`` via       |assignment  |TypeMismatch|
|            |                     |extract<> or    |            |error       |
|            |                     |conversion error|            |            |
+------------+---------------------+----------------+------------+------------+
|``U``       |ValueNone error      |``U`` via       |TypeMismatch|assignment  |
|            |                     |extract<> or    |error       |            |
|            |                     |conversion error|            |            |
+------------+---------------------+----------------+------------+------------+


.. python api
   ----------
   .. autoclass:: ecto.Tendril
       :members:

.. c++ api
   -------
   .. doxygenclass:: ecto::tendril
       :members:

.. _spore-overview:

spore
=====

A spore is a typed handle for a tendril: it is basically a typed pointer that wraps a
tendril. Its use is purely for ease of coding: instead of extracting the object from a
tendril every time, you can just define a spore. You can think of a tendril as 
containing a ``void*`` and a spore as a cast of that pointer to a specific type.

.. c++ api
   -------
   .. doxygenclass:: ecto::spore
       :members:

.. _tendrils:

.. _tendrils-overview:

tendrils
========

tendrils are containers for the tendril type, essentially a mapping from name to tendril.
The tendrils also give a convenient form of templated type safe access to the data that
tendril objects hold.

.. python api
   ----------
   .. autoclass:: ecto.Tendrils

   c++ api
   -------
   .. doxygenclass:: ecto::tendrils
       :members:


.. _plasm-overview:

plasm
=====

from the Greek *plasma*, meaning "something formed or molded"
Think ectoplasm or protoplasm. Leading naturally to slime molds. The plasm
is the ecto **Directed Acyclic Graph**, or *DAG* for short.


.. Python interface
   ----------------

   (This is autogenerated from python bindings... FIX ME this is ugly)

   .. autoclass:: ecto.Plasm
      :members:

   C++ interface
   -------------

   (Autogenerated from doxygen output)

   .. doxygenstruct:: ecto::plasm
