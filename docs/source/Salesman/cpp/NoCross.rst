NoCross
=======

.. image:: ../../../html/class_no_cross.png
    :alt: Class diagram for NoCross

The NoCross class further improves the result of the Shortest class.
Iterating through the path and detecting crossing edges in the path graph,
and subsequently untangling these crossing paths. 
This is repeated until no crossing edges are left.

.. doxygenclass:: NoCross
    :members:

Utility Classes
---------------

NoCross algorithm requires two additional classes, Point and Edge.
These are briefly shown below:

The Point Class contains the x and y coordinate of a point as well as the corresponding index.

.. doxygenclass:: Point
    :members:

The Edge Class is used to link two points in a path together.
Its methods are used to check if the path has crossing edges.

.. doxygenclass:: Edge
    :members:

