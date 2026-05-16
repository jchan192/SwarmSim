import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
from torch.distributions import Normal
import os
import sys
import random
sys.path.append(r"C:\Users\jpjch\swarm_simulation\x64\Release")
import swarm_simulation_py

maxSpeed = swarm_simulation_py.GetAgentMaxSpeed();
numFrames = 100
# Change this to however many output files you have
#outputDir = r"C:\Users\jpjch\swarm_simulation\Output_Data"
#outputDir = r"C:\Users\jpjch\swarm_simulation\Output_Data\dest_x2.0_y0.5"
outputDir = r"C:\Users\jpjch\swarm_simulation\Output_Data\dest_x0.5_y1.5"
#outputDir = r"C:\Users\jpjch\swarm_simulation\Output_Data\dest_x1.8_y2.0"


fig, ax = plt.subplots()

destination = ax.scatter([], [])
scatter = ax.scatter([], [])
objectScatter = ax.scatter([], [], s=25, marker="s", label="Object")

ax.set_xlim(-0.25, 2.5)
ax.set_ylim(-0.25, 2.5)
ax.set_xlabel("x")
ax.set_ylabel("y")
ax.grid(True)

def update(frame_i):
    frame_i *= 2
    fileName = os.path.join(outputDir, f"agentsPhaseSpace_{frame_i}.dat")
    data = np.loadtxt(fileName, skiprows=1)

    objectFileName = os.path.join(outputDir, f"objectPhaseSpace_{frame_i}.dat")
    objectData = np.loadtxt(objectFileName, skiprows=1)
    
    # data columns: id, x, y, vx, vy
#    x = data[:, 1]
#    y = data[:, 2]
    x = data[1]
    y = data[2]

    scatter.set_offsets(np.column_stack((x, y)))
    objectScatter.set_offsets(np.column_stack((objectData[:,1], objectData[:,2])))
#    destination.set_offsets([[2.0, 0.5]],)
    destination.set_offsets([[0.5, 1.5]],)
#    destination.set_offsets([[1.8, 2.0]],)

    ax.set_title(f"Time step {frame_i}")

    return scatter, objectScatter, destination

ani = animation.FuncAnimation(
    fig,
    update,
    frames=numFrames,
    interval=100,
    blit=False
)

plt.show()