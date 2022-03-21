import sys
import subprocess

def createMatrix (filename):
    i = 0
    with open(filename) as fp:
        N = len(fp.readlines())
        fp.seek(0)
        matrix = [ [0]*N for _ in range(N) ]
        while True:
            line = fp.readline()
            if not line:
                break
            for x in line.split()[1:]:
                matrix[i][int(x) - 1] = 1
            i = i + 1
    return matrix

def graphColoring (matrix, N):
    exec_file = "./gcolorpy"
    proc = subprocess.Popen([exec_file],stdin=subprocess.PIPE,stdout=subprocess.PIPE)

    # Convert input matrix to adjacency list
    for i in range(N):
        counter = 0
        for j in range(N):
            if (matrix[i][j] == 1):
                if (counter > 0):
                    proc.stdin.write (' '.encode("utf-8"))
                counter = counter + 1
                proc.stdin.write ((str(j)).encode("utf-8"))
            if (j == N - 1):
                proc.stdin.write ('\n'.encode("utf-8"))
    proc.stdin.close()

    colors=[]
    while True:
        line = proc.stdout.readline()
        if not line:
            break
        group = []
        for x in line.split():
            group.append(int(x))
        colors.append(group)
    proc.stdout.close()
    return colors

if __name__=="__main__":
    m = createMatrix (sys.argv[1])
    print (graphColoring (m, len(m)))
