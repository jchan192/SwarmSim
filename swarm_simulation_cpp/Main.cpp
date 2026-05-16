#include "..\swarm_simulation\SwarmSim.h"

int main() {

    std::vector<float> destinationxy, objectxy;
    destinationxy.push_back(1.5);
    destinationxy.push_back(1.5);
    objectxy.push_back(1.0);
    objectxy.push_back(1.0);
    ResetSwarmSimulation(destinationxy, objectxy);

    for (int timeStep = 1; timeStep < m_totalTimeSteps; timeStep++) {
        EvolveSwarmSimulation();
        WriteAgentsToFile(m_agents, timeStep);
        if (!m_object.objectPoints.empty()) {
            WriteAllToFile(timeStep);
        }
    }
    return 0;
}