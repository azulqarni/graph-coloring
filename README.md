# Wigderson’s Graph Coloring Approximation Algorithm
Wigderson Graph Coloring algorithm implementation along with a Python wrapper file for invocation of the algorithm's C implementation.

- `graphgen.c` containts the code for the production of a random input graph with number of vertices specified by the first argument passed to the executable.
- `gcolor.c` contains the algorithm imlementation in C accepting a graph that is stored in a adjacent list format. In each of the input graphs, vertices are numbered from 1 to |V|. Each row includes a vertex, followed by zero or more other vertices. For example:<br /><br />
1 3 6<br />
2 3 5<br />
3 1 2 4 5<br />
4 3 5<br />
5 2 3 4 6<br />
6 1 5<br /><br />
This input should be interpreted as follows: the undirected edges of G are (1,3), (1,6), (2,3), (2,5), (3,4), (3,5), (4,5) and (5,6). The vertices following the source vertex are in arbitrary order. The output consists of vertices grouped by color, with all vertices assigned with the same color printed in the same line of the output. In the first line, the number of colors used is printed.
- `gcolorpy.c` contains the algoritmh implementation compatible with the python wrapper accepting the input graph as lines of adjacency lists, where vertices are enumerated with consecutive numbers starting from 0.
- `wrapper.py` contains the Python wrapper script which provides the a function for the `gcolorpy` program to be invoked on a graph that is stored in adjacency matrix format, featuring interprocess communication, which returns vertices grouped by color in one list of lists.
- Run `setup.sh` to compile and test the above script and programs.