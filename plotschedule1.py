import matplotlib.pyplot as plt
from math import sqrt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.patches import FancyArrowPatch
from mpl_toolkits.mplot3d import proj3d
from numpy import arange, ones

class Arrow3D(FancyArrowPatch):
    def __init__(self, xs, ys, zs, *args, **kwargs):
        FancyArrowPatch.__init__(self, (0,0), (0,0), *args, **kwargs)
        self._verts3d = xs, ys, zs

    def draw(self, renderer):
        xs3d, ys3d, zs3d = self._verts3d
        xs, ys, zs = proj3d.proj_transform(xs3d, ys3d, zs3d, renderer.M)
        self.set_positions((xs[0],ys[0]),(xs[1],ys[1]))
        FancyArrowPatch.draw(self, renderer)

def plotschedule(filename):
    f = open(filename, 'r')
    cost = f.readline()
    mint = 1000.0
    maxt = 0
    minnode = 1000
    maxnode = 0
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    ax.set_title("Flight scheduling : Time-space plot")
    ax.set_xlabel('times')
    ax.set_ylabel('nodes')
    ax.set_zlabel('flights')
    ax.grid(b=True, which='major')
    colours = [ 'b', 'g', 'r', 'c', 'm', 'y', 'k', 'DarkOrange', 'ForestGreen', 'Indigo ', 'MediumAquaMarine', 'MidnightBlue', 'Teal', 'SteelBlue', 'LightPink']
    count = 0
    for line in f:
        elems = line.split(':')
        flight = int(elems[0].strip())
        elems = elems[1].split('-')
        nodes = []
        times = []
        for elem in elems:
            elem = elem.strip()
            elem = elem.split()
            node = float(elem[0].strip())
            time = float(elem[1].strip())
            if node > maxnode: maxnode = node
            if node < minnode: minnode = node
            if time > maxt: maxt = time
            if time < mint: mint = time
            nodes.append(node)
            times.append(time)
        for i in range(len(nodes)-1):
            a = Arrow3D([times[i], times[i+1]],[nodes[i], nodes[i+1]],[count, count], mutation_scale=20, lw=1, arrowstyle="-|>", color=colours[count%len(colours)])
            ax.add_artist(a)
        ax.scatter(times, nodes, ones(len(times))*count, 'o', color=colours[count%len(colours)])
        count+=1
    ax.set_yticks(arange(minnode, maxnode+1))
    ax.set_zticks(arange(0, count+1))
    ax.set_xticks(arange(mint, maxt+1))
    ax.set_ylim(minnode, maxnode+1)
    ax.set_zlim(0, count)
    ax.set_xlim(mint, maxt+1)  
    plt.show()

if __name__ == "__main__":                
    plotschedule('sol4.txt')
