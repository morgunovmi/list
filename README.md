# List

### A cache-friendly array-based doubly linked list implementation in C-style C++.

This list supports fast element insertion and access provided that you have its physical index. 
A slow but more convenient logical index based access is also possible.

The list can also be sorted so that physical indices are the same as logical 
for fast and covenient access. (Keep in mind that after the sort all previous physical indices are invalidated).

The list also has a graphical dump function that uses graphviz for easier visualisation and debug. 
