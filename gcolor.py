import sys
import networkx as nx
import matplotlib as mpl
import matplotlib.pyplot as plt

# Read input file
fp = open(sys.argv[1], 'r')
Lines = fp.readlines()
fp.close()

# Parse lines and create graph
n = 0
G = nx.Graph()
for line in Lines:
    line = line.strip().split(' ')
    n = n + 1
    v = line[0]
    line.pop(0)
    for i in line:
        G.add_edge(v, i)

# Draw graph and save
#nx.draw_spring(G, with_labels = True)
#plt.savefig('input_graph.png')

d = nx.coloring.greedy_color(G, strategy="largest_first")
print (d)

