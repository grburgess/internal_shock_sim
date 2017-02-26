import numpy as np
import matplotlib.pyplot as plt
from glob import glob
from spectralTools.spectralPlotter.movieMaker import movieMaker

c=2.99E10

#TrajectoryPlotter
def TrajectoryPlotter(fileName):
    
    #Setup figure
    fig=plt.figure(1)
    ax=fig.add_subplot(111)
    ax.set_autoscale_on(False)

    #grab radii
    radii = np.genfromtxt(fileName,names=True)['radius']
    gamma = np.genfromtxt(fileName,names=True)['gamma']
    
    maxR=radii.max()
    minR=radii.min()
    radii = radii/maxR
    
    delta = radii.max()-radii.min()
    #print delta
    y=.25*delta

    


    For r,g in zip(radii[:-1],gamma[:-1]):
        if g > 200:
            c='r'
        else:
            c='b'


        ax.axvline(x=r,linewidth = .2, color=c,ymin=-1,ymax=1)


    
    ax.set_xlim([radii.min(),radii.max()])
    #ax.set_xlim([.5,1.])
    ax.set_ylim([-y,y])
    
    ax.set_aspect(.2)
    return figarray([data['radius'],data['mass'],data['gamma']])
    
def GammaMTot(fileName):

    fig=plt.figure(2)
    ax=fig.add_subplot(111)
    f=np.genfromtxt(fileName,names=True)
    gamma = f['gamma']
    mass  = f['mass']
    totMass = mass.sum()
    ax.plot(mass.cumsum()/totMass,gamma,'r.')

def ArrivalTime(t,r):
    dist = 5.8E27
    ta= t-(r/c)
    return ta



#Load and sort files
unSortedFiles = glob("traj_*.txt")
nums=[]
for x in unSortedFiles:
    nums.append(int(x[5:][:-4]))


nums=np.array(nums)
nums.sort()
sortedFiles=[]
for n in nums:
    for f in unSortedFiles:
        if int(f[5:][:-4])==n:
            sortedFiles.append(f)

del unSortedFiles
del nums
trajFiles = sortedFiles

#Get Collisions
collisions = np.genfromtxt("collision_hist.txt",names=True)



#mm=movieMaker()
#for f in sortedFiles:
#    fig = TrajectoryPlotter(f)
#    mm.FigSave(fig)
#    fig.clf()
#mm.CreateMovie(fps=5.)
    



    


