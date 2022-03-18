# Wigderson’s Graph Coloring Approximation Algorithm
Wigderson Graph Coloring algorithm implementation along with a Python wrapper file for invocation of the algorithm's C implementation.

- `graphgen.c` containts the code for the production of a random input graph with number of vertices specified by the first argument.
- `gcolor.c` contains the algorithm imlementation in C accepting a graph that is stored in a adjacent list format. In each of the input graphs, vertices are numbered from 1 to |V|. Each row includes a vertex, followed by zero or more other vertices. For example:<br />
1 3 6<br />
2 3 5<br />
3 1 2 4 5<br />
4 3 5<br />
5 2 3 4 6<br />
6 1 5<br />
This input should be interpreted as follows: the undirected edges of G are (1,3), (1,6), (2,3), (2,5), (3,4), (3,5), (4,5) and (5,6). The vertices following the source vertex are in arbitrary order. There are 5 input graphs provided.
- `gcolorpy.c` contains the algoritmh implementation compatible with the python wrapper accepting the input graph as lines of adjacency lists, where vertices are enumerated with consecutive numbers starting from 0.
- `wrapper.py` contains the Python wrapper script which provides the a function for the `gcolorpy` program to be invoked on a graph that is stored in adjacency matrix format, featuring interprocess communication.
- Run `setup.sh` to compile and test the above script and programs.