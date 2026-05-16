# This code calls the physics simulation from SwarmSim.cpp and train it here

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
destinationxy = [1.8,2.0]
objectxy = [1.0,1.0]
numFrames = 200

class PolicyNet(nn.Module):
    def __init__(self, obs_dim=10, hidden_dim=64, action_dim=2):
        super().__init__()

        self.net = nn.Sequential(
            nn.Linear(obs_dim, hidden_dim),
            nn.Tanh(),
            nn.Linear(hidden_dim, hidden_dim),
            nn.Tanh(),
            nn.Linear(hidden_dim, action_dim),
            nn.Tanh()
        )

        # controls exploration/randomness
        self.log_std = nn.Parameter(torch.zeros(action_dim))

    def forward(self, obs):
        mean = self.net(obs)
        std = torch.exp(self.log_std)
        return Normal(mean, std)

def compute_returns(rewards, gamma=0.99):
    returns = []
    G = 0.0

    for r in reversed(rewards):
        G = r + gamma * G
        returns.insert(0, G)

    returns = torch.tensor(returns, dtype=torch.float32)

    if len(returns) > 1:
        returns = (returns - returns.mean()) / (returns.std() + 1e-8)

    return returns
def run_trained_policy(policy, max_steps=300, max_speed=maxSpeed, write_output=True):
    results = swarm_simulation_py.Reset(destinationxy, objectxy)
    numAgents = int(swarm_simulation_py.GetNumberOfAgents())

    if write_output:
        swarm_simulation_py.WriteAll(0)

    total_reward = 0.0

    for step in range(max_steps):
        tensorState = torch.tensor(
            results.stateOfSim,
            dtype=torch.float32
        ).view(numAgents, 10)

        # no training during output/test
        with torch.no_grad():
            dist = policy(tensorState)

            # use mean action, not random sample
            action = dist.mean

            velocity = torch.clamp(action, -1.0, 1.0) * max_speed
            actions = velocity.numpy().flatten().tolist()

        swarm_simulation_py.ApplyAction(actions)
        results = swarm_simulation_py.Evolve(False)

        total_reward += results.reward

        if write_output:
            swarm_simulation_py.WriteAll(step + 1)

        print(
            "test step:", step,
            "reward:", results.reward,
            "total_reward:", total_reward,
            "achieved:", results.achieved,
            "object-dest:",swarm_simulation_py.GetDistanceFromObjectToDestination(),
        )

        if results.achieved:
            print("objective achieved in test!")
        #     break

    return total_reward, results.achieved

# initialize NN
policy = PolicyNet()
# policy.load_state_dict(torch.load("best_swarm_policy.pth"))
# policy.eval()
optimizer = optim.Adam(policy.parameters(), lr=1e-3)
numOfTraining = 25000
#numOfTraining = 1

best_reward = -1e9
best_model_state = None

for episode in range(numOfTraining):

    # for NN
    log_probs = []
    rewards = []
    total_reward = 0.0
    done = False

    results = swarm_simulation_py.Reset(destinationxy, objectxy)
    swarm_simulation_py.WriteAll(0);
    numAgents = np.int16(swarm_simulation_py.GetNumberOfAgents());

    # run full swarm simulation
    for step in range(numFrames):

        # get action from NN 
        tensorState = torch.tensor(results.stateOfSim, dtype=torch.float32).view(numAgents, 10)
        dist = policy(tensorState)
        action = dist.sample()
        velocity = torch.clamp(action, -1.0, 1.0) * maxSpeed
        actions = velocity.detach().numpy().flatten().tolist()

         # save log probability for training
        log_prob = dist.log_prob(action).sum(dim=1).mean()
        log_probs.append(log_prob)

        # evolve simulation
        swarm_simulation_py.ApplyAction(actions)
        results = swarm_simulation_py.Evolve(False);

        # record the reward at every step for training
        done = results.achieved
        rewards.append(results.reward)
        total_reward += results.reward

        if ( results.achieved ):
            print( "objective achieved !")
#            break

    # Give a a deterministic test to check if we have best_model now or not. If yes, use it
    if episode % 50 == 0:
        test_reward, test_achieved = run_trained_policy( policy, max_steps=numFrames, max_speed=maxSpeed, write_output=True )

        if test_reward > best_reward:
            best_reward = test_reward
            best_model_state = {
                k: v.detach().clone() for k, v in policy.state_dict().items()
            }
        print("deterministic test reward:", test_reward, "achieved:", test_achieved)

    # train AFTER the episode
    returns = compute_returns(rewards)

    loss = 0.0
    for log_prob, G in zip(log_probs, returns):
        loss += -log_prob * G

    optimizer.zero_grad()
    loss.backward()
    optimizer.step()
    print(
        "episode:", episode,
        "total_reward:", total_reward,
        "steps:", step + 1,
        "done:", done
    )

policy.load_state_dict(best_model_state) # run using the best state
torch.save(best_model_state, "best_swarm_policy.pth")
total_reward, achieved = run_trained_policy(policy,numFrames, maxSpeed)
print("final test total reward:", total_reward, "achieved:", achieved)

# Change this to however many output files you have
outputDir = r"C:\Users\jpjch\swarm_simulation\Output_Data"


fig, ax = plt.subplots()

scatter = ax.scatter([], [])
objectScatter = ax.scatter([], [], s=25, marker="s", label="Object")

ax.set_xlim(-0.25, 3.05)
ax.set_ylim(-0.25, 3.05)
ax.set_xlabel("x")
ax.set_ylabel("y")
ax.grid(True)

def update(frame_i):
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

    ax.set_title(f"Time step {frame_i}")

    return scatter, objectScatter

ani = animation.FuncAnimation(
    fig,
    update,
    frames=numFrames,
    interval=100,
    blit=True
)

plt.show()