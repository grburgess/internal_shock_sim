from numpy import linspace, sqrt, array, cos, pi, arange, piecewise
c=2.99E10





TOTAL_ENERGY = 2*1.E51/(4* pi)



dT = .005
t0 = 0.
Rmin = 1.2E4
#iTime = linspace(T0+dT,0.,NUM_SHELLS)
tw=10.

NUM_SHELLS = int(tw/dT)

dE = float(TOTAL_ENERGY)/float(NUM_SHELLS)
def GammaDist2(t):
    
    if t<=0.4*tw:
        val = 250.-150.* cos(pi*t/(.4*tw))
    else:
        val =400.

    return val

def GammaDist(i):
    
    if i>=100:
        gamma = 400.

    else:
        gamma = 100.

    return gamma


iGamma=[]

for t in arange(t0,tw,dT):

    iGamma.append(GammaDist2(t))


def RadialDist(i):
    
    v=c*sqrt((iGamma[i]**2)-1.)/iGamma[i]
    
    return -iTime[i]*v


def RadialDist2(i):
    
    v=c*sqrt((iGamma[i]**2)-1.)/iGamma[i]
    
    return Rmin+arange(t0,tw,dT)[i]*v



    

iRad = []

for i in range(NUM_SHELLS-1,-1,-1):

   
    iRad.append(RadialDist2(i))



    
    
   
    

iGamma = array(iGamma)

iMass = dE/(iGamma*(c**2))



f=open("initialize.txt",'w')

for t,m,g,r in zip(arange(t0,tw,dT),iMass,iGamma,iRad):

    row = str(r)+"\t"+str(t)+"\t"+str(g)+"\t"+str(m)+'\n'
    f.write(row)
f.close()    











#for i in range(NUM_SHELLS):

    








