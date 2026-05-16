import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("../tmpOutput/data.dat")
x = np.linspace(0,data.shape[0],data.shape[0])
plt.plot(x, data[:,0], '-', label = "progess-object-dest-distance")
#plt.plot(x, data[:,1], '-', label = "objective-object-dest-distance")
plt.plot(x, data[:,2], '-', label = "dest-distance")
plt.plot(x, data[:,3], '-', label = "progress-close-to-pushing-point")
#plt.plot(x, data[:,4], '-', label = "objective-close-to-pushing-point")
plt.plot(x, data[:,5], '-', label = "pushing-point-distance")
#plt.plot(x, data[:,6], '-', label = "velocity-alignment")
plt.plot(x, np.sum(data[:,np.array([0,3])],axis=1), '--', label = 'total')
plt.legend()
plt.show()
