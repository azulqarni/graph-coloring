# Wigderson’s Approximate Graph Coloring Algorithm
This is a Wigderson’s approximate graph coloring algorithm implementation
along with a Python wrapper file for invocation of the algorithm's C
implementation.

- `graphgen.c` containts the code for the production of a random input graph
with number of vertices specified by the first argument passed to the
executable.
- `gcolor.c` contains the algorithm imlementation in C accepting a graph that
is stored in a adjacent list format. In each of the input graphs, vertices are
numbered from 1 to |V|. Each row includes a vertex, followed by zero or more
other vertices. For example:<p>
1 3 6<br />
2 3 5<br />
3 1 2 4 5<br />
4 3 5<br />
5 2 3 4 6<br />
6 1 5</p>
This input should be interpreted as follows: the undirected edges of G are
(1,3), (1,6), (2,3), (2,5), (3,4), (3,5), (4,5) and (5,6). The vertices
following the source vertex are in arbitrary order. The output consists of
vertices grouped by color, with all vertices assigned with the same color
printed in the same line of the output. In the first line, the number of colors
used is printed. If `gcolor.c` is compiled  with `-DATTACHED` an executable
compatible with the python wrapper is produced, accepting the input graph as
lines of adjacency lists, where vertices are enumerated with consecutive numbers
starting from 0.
- `wrapper.py` contains the Python wrapper script which provides the a function
for the compatible program to be invoked on a graph that is stored in adjacency
matrix format, featuring interprocess communication, which returns vertices
grouped by color in one list of lists.
- Run `setup.sh` to compile and test the above script and programs.

#### Performance Comparison for |V|=8000 (x7.6 Speed-Up)
Implementation @`Intel(R) Xeon(R) CPU E3-1245 v5 @ 3.50GHz`| Measured Time (s) | Colors Returned
:----------------------------------------------------------|-------------------|----------------:
Current Wigderson’s in Algorithm C                         |   7.009           | 719
NetworkX (`strategy_largest_first`) in Python              |   53.277          | 726
