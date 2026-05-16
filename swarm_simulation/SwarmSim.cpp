#include <iostream>
#include <fstream>
#include <cmath>
#include "SwarmSim.h"
#include <random>

std::string m_outputDir = "C:\\Users\\jpjch\\swarm_simulation\\Output_Data";
float m_distanceBetweenAgents = (float) 0.05;
float m_minBoundaryX = (float) 0;
float m_maxBoundaryX = (float) 3;
float m_minBoundaryY = (float) 0;
float m_maxBoundaryY = (float) 3;
float m_agentSize = (float) 0.05;
float m_maxSpeed = (float) 0.35;
int m_totalTimeSteps = 100;
std::vector<float> m_destinationxy;

//std::vector<float> m_destinationxy = { 2.0 ,2.0 };
//std::vector<float> m_destinationxy = { 0.3,1.8 };
//std::vector<float> m_destinationxy = { 0.6,0.7 };

float m_oldDistanceToDestination, m_oldDistanceToAgents;
std::vector<AgentStruct> m_agents;
ObjectStruct m_object;

ObjectStruct InitializeObject(const std::vector<float>& objectxy ) {

    std::vector<AgentStruct> objectPoints;

    const int numberOfObjectAgents = 40;

    const float objectWidth  = 0.3f;
    const float objectHeight = 0.3f;

    const float halfWidth  = objectWidth / 2.0f;
    const float halfHeight = objectHeight / 2.0f;

//    const float centerX = 0.5f * (m_minBoundaryX + m_maxBoundaryX);
//    const float centerY = 0.5f * (m_minBoundaryY + m_maxBoundaryY);
    const float centerX = objectxy[0] ;
    const float centerY = objectxy[1] ;

    const float left   = centerX - halfWidth;
    const float right  = centerX + halfWidth;
    const float bottom = centerY - halfHeight;
    const float top    = centerY + halfHeight;

    int id = 0;

    auto AddObjectAgent = [&](float x, float y) {
        AgentStruct objectAgent;

        objectAgent.id = id++;
        objectAgent.pos.x = x;
        objectAgent.pos.y = y;

        // Static object
        objectAgent.vel.x = 0.0f;
        objectAgent.vel.y = 0.0f;
        objectAgent.acc.x = 0.0f;
        objectAgent.acc.y = 0.0f;

        objectPoints.push_back(objectAgent);
    };

    // Number of points per side.
    // Corners are included only once.
    const int bottomCount = 6;
    const int rightCount  = 4;
    const int topCount    = 6;
    const int leftCount   = 4;

    // Bottom edge: left -> right
    for (int i = 0; i < bottomCount; i++) {
        float t = static_cast<float>(i) / static_cast<float>(bottomCount);
        float x = left + t * objectWidth;
        float y = bottom;
        AddObjectAgent(x, y);
    }

    // Right edge: bottom -> top
    for (int i = 0; i < rightCount; i++) {
        float t = static_cast<float>(i) / static_cast<float>(rightCount);
        float x = right;
        float y = bottom + t * objectHeight;
        AddObjectAgent(x, y);
    }

    // Top edge: right -> left
    for (int i = 0; i < topCount; i++) {
        float t = static_cast<float>(i) / static_cast<float>(topCount);
        float x = right - t * objectWidth;
        float y = top;
        AddObjectAgent(x, y);
    }

    // Left edge: top -> bottom
    for (int i = 0; i < leftCount; i++) {
        float t = static_cast<float>(i) / static_cast<float>(leftCount);
        float x = left;
        float y = top - t * objectHeight;
        AddObjectAgent(x, y);
    }

    // set object global property
    ObjectStruct object;
    object.M = 10.0;
    object.centerPos.x = centerX;
    object.centerPos.y = centerY;
    object.centerVel.x = (float) 0.0;
    object.centerVel.y = (float) 0.0;
    object.centerAcc.x = (float) 0.0;
    object.centerAcc.y = (float)0.0;
    object.objectPoints = objectPoints;

    return object;
}

std::vector<AgentStruct> InitializeAgent() {
    
    int numberOfAgents = 1;
    std::vector<AgentStruct> agents;
    for (int agent_i = 0; agent_i < 1; agent_i++) {
        AgentStruct agent;
        agent.id = agent_i;
        //agent.pos.x = (float) ( 0.3 + agent_i * 0.3 );
        //agent.pos.y = (float) (0.1 );
        agent.pos.y = (float) ( 0.3 + agent_i * 0.3 );
        agent.pos.x = (float) (0.1 );
        agent.vel.x = (float) 0;
        agent.vel.y = (float) 0;
        agent.acc.x = (float) 0;
        agent.acc.y = (float) 0;
        agents.push_back(agent);
    }
    //for (int agent_i = 4; agent_i < 8; agent_i++) {
    //    AgentStruct agent;
    //    agent.id = agent_i;
    //    agent.pos.y = (float) ( 0.3 + agent_i * 0.3 );
    //    agent.pos.x = (float) (0.1 );
    //    agent.vel.x = (float) 0;
    //    agent.vel.y = (float) 0;
    //    agent.acc.x = (float) 0;
    //    agent.acc.y = (float) 0;
    //    agents.push_back(agent);
    //}

//    int numberOfAgents = 15;
//    std::vector<AgentStruct> agents;
//
//    std::random_device rd;
//    std::mt19937 gen(rd());
//
//    float halfAgentSize = m_agentSize / 2.0f;
//
//    std::uniform_real_distribution<float> posXDist(
//        m_minBoundaryX + halfAgentSize,
//        1.0 - halfAgentSize
////        m_maxBoundaryX - halfAgentSize
//    );
//
//    std::uniform_real_distribution<float> posYDist(
//        m_minBoundaryY + halfAgentSize,
//        m_maxBoundaryY - halfAgentSize
//    );
//
//    std::uniform_real_distribution<float> velDist(-0.2f, 0.2f);
//
//    for (int agent_i = 0; agent_i < numberOfAgents; agent_i++) {
//        AgentStruct agent;
//
//        agent.id = agent_i;
//        agent.m = (float) 1.0;
//
//        agent.pos.x = posXDist(gen);
//        agent.pos.y = posYDist(gen);
//
//        agent.vel.x = velDist(gen);
//        agent.vel.y = velDist(gen);
////        agent.vel.x = 0.0;
////        agent.vel.y = 0.0;
//
//        agent.acc.x = (float) 0;
//        agent.acc.y = (float) 0;
//
//        agents.push_back(agent);
//    }

    return agents;
}

void WriteObjectToFile(ObjectStruct object, const int timeStep) {

    if (object.objectPoints.empty()) {
        std::cout << "Nothing to write for object!!\n";
        return;
    }

    std::string fileName = m_outputDir + "\\objectPhaseSpace_" + std::to_string(timeStep) + ".dat";
    std::ofstream outputFile(fileName);

    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return;
    }

    // Header
    outputFile << "id x y vx vy\n";

    for (const auto& obPoints : object.objectPoints) {
        outputFile << obPoints.id << " "
                   << obPoints.pos.x << " "
                   << obPoints.pos.y << " "
                   << obPoints.vel.x << " "
                   << obPoints.vel.y << "\n";
    }

    outputFile.close();
}

void WriteAgentsToFile(const std::vector<AgentStruct>& agents, const int timeStep) {

    if (agents.empty()) {
        std::cout << "Nothing to write for agents !!\n";
        return;
    }

    std::string fileName = m_outputDir + "\\agentsPhaseSpace_" + std::to_string(timeStep) + ".dat";
    std::ofstream outputFile(fileName);

    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return;
    }

    // Header
    outputFile << "id x y vx vy\n";

    for (const auto& agent : agents) {
        outputFile << agent.id << " "
                   << agent.pos.x << " "
                   << agent.pos.y << " "
                   << agent.vel.x << " "
                   << agent.vel.y << "\n";
    }

    outputFile.close();
}

void WriteAllToFile(const int timeStep) {
    WriteAgentsToFile(m_agents, timeStep);
    WriteObjectToFile(m_object, timeStep);
    
}
// Example in x-direction only:
// 
//   +--------------------------+
//   |             i            |
//   |                          |
//   +--------------------------+
//                    +--------------------------+
//                    |             j            |
//                    |                          |
//                    +--------------------------+
//                  [- distanceij -]       
//                    [--------agentSize ---------]   square j width
//
// there is overlap if  agentSize > distanceij_x

void ComputeAcceleration(std::vector<AgentStruct>& agents, ObjectStruct& object) {

    float springConstant = 50.0;

    int numberOfAgents = agents.size();
    float halfAgentSize = (float)m_agentSize / 2.;
    float factor = 1.0;
    int numberOfObject = object.objectPoints.size();

    // reset acceleration
    object.centerAcc.x = (float) 0.0;
    object.centerAcc.y = (float) 0.0;

    for (int agent_i = 0; agent_i < numberOfAgents; agent_i++) {
        for (int object_i = 0; object_i < numberOfObject; object_i++) {
            float distanceij_y = std::fabs(object.objectPoints[object_i].pos.y - agents[agent_i].pos.y);
            float distanceij_x = std::fabs(object.objectPoints[object_i].pos.x - agents[agent_i].pos.x);
            if (distanceij_x < m_agentSize && distanceij_y < m_agentSize) {
                if (distanceij_y > distanceij_x) {
                    // only do y-correction
                    if (agents[agent_i].pos.y > object.objectPoints[object_i].pos.y) {
//                        agents[agent_i].acc.y += springConstant * (agentSize - distanceij_y) / (float) 2.; 
                          object.centerAcc.y -= springConstant * (m_agentSize - distanceij_y) / (float) 2.;
                    } else {
//                      agents[agent_i].acc.y -= springConstant * (agentSize - distanceij_y) / (float) 2.; 
                        object.centerAcc.y += springConstant * (m_agentSize - distanceij_y) / (float) 2.;
                    }
                } else {
                    // only do x-correction
                    if (agents[agent_i].pos.x > object.objectPoints[object_i].pos.x) {
//                        agents[agent_i].pos.x += (agentSize - distanceij_x) / (float)2. * factor;
                        object.centerAcc.x -= springConstant * (m_agentSize - distanceij_x) / (float) 2.;
                    }
                    else {
//                        agents[agent_i].pos.x -= (agentSize - distanceij_x) / (float)2. * factor;
                        object.centerAcc.x += springConstant * (m_agentSize - distanceij_x) / (float) 2.;
                    }
                }
            }
        }
    }
}

void ApplyConstraints( std::vector<AgentStruct>& agents, ObjectStruct& object) {

    int numberOfAgents = agents.size();
    float halfAgentSize = (float) m_agentSize / 2.;
    float factor = 1.0;
    int numberOfIterations = 25;
    bool noMoreOverlaps = true;

    // Get the acceleration first before applying the hard constraints
    ComputeAcceleration(agents, object);

    for (int iter = 0; iter < numberOfIterations; iter++) {
        // reset noMoreOverlaps
        noMoreOverlaps = true;

        for (int agent_i = 0; agent_i < numberOfAgents; agent_i++) {


            // apply agent-agent constraints
            for (int agent_j = agent_i + 1; agent_j < numberOfAgents; agent_j++) {

                float distanceij_y = std::fabs(agents[agent_j].pos.y - agents[agent_i].pos.y);
                float distanceij_x = std::fabs(agents[agent_j].pos.x - agents[agent_i].pos.x);                
                if (distanceij_x < m_agentSize && distanceij_y < m_agentSize) {
                    if (distanceij_y > distanceij_x) {
                        // only do y-correction
                        if (agents[agent_i].pos.y > agents[agent_j].pos.y) {
                            agents[agent_i].pos.y += (m_agentSize - distanceij_y) / (float)2. * factor;
                            agents[agent_j].pos.y -= (m_agentSize - distanceij_y) / (float)2. * factor;
                        }
                        else {
                            agents[agent_j].pos.y += (m_agentSize - distanceij_y) / (float)2. * factor;
                            agents[agent_i].pos.y -= (m_agentSize - distanceij_y) / (float)2. * factor;
                        }
                    } else {
                        // only do x-correction
                        if (agents[agent_i].pos.x > agents[agent_j].pos.x) {
                            agents[agent_i].pos.x += (m_agentSize - distanceij_x) / (float)2. * factor;
                            agents[agent_j].pos.x -= (m_agentSize - distanceij_x) / (float)2. * factor;
                        }
                        else {
                            agents[agent_j].pos.x += (m_agentSize - distanceij_x) / (float)2. * factor;
                            agents[agent_i].pos.x -= (m_agentSize - distanceij_x) / (float)2. * factor;
                        }
                    }
                    noMoreOverlaps = false;
                }
            }

            // apply agent-object constraints
            int numberOfObject = object.objectPoints.size();
            for (int object_i = 0; object_i < numberOfObject; object_i++) {
                float distanceij_y = std::fabs(object.objectPoints[object_i].pos.y - agents[agent_i].pos.y);
                float distanceij_x = std::fabs(object.objectPoints[object_i].pos.x - agents[agent_i].pos.x);                
                if (distanceij_x < m_agentSize && distanceij_y < m_agentSize) {
                    if (distanceij_y > distanceij_x) {
                        // only do y-correction
                        if (agents[agent_i].pos.y > object.objectPoints[object_i].pos.y) {
                            agents[agent_i].pos.y += (m_agentSize - distanceij_y) / (float)2. * factor;
                        }
                        else {
                            agents[agent_i].pos.y -= (m_agentSize - distanceij_y) / (float)2. * factor;
                        }
                    } else {
                        // only do x-correction
                        if (agents[agent_i].pos.x > object.objectPoints[object_i].pos.x) {
                            agents[agent_i].pos.x += (m_agentSize - distanceij_x) / (float)2. * factor;
                        }
                        else {
                            agents[agent_i].pos.x -= (m_agentSize - distanceij_x) / (float)2. * factor;
                        }
                    }
                    noMoreOverlaps = false;
                }               
            }

            // apply boundary constraints 
            //float overlap_minbound_y = std::fabs(agents[agent_i].pos.y - m_minBoundaryY);
            //if (overlap_minbound_y < halfAgentSize) {
            //    agents[agent_i].pos.y += (halfAgentSize - overlap_minbound_y) * factor;
            //    noMoreOverlaps = false;
            //}
            //float overlap_maxbound_y = std::fabs(agents[agent_i].pos.y - m_maxBoundaryY);
            //if (overlap_maxbound_y < halfAgentSize) {
            //    agents[agent_i].pos.y -= (halfAgentSize - overlap_maxbound_y) * factor;
            //    noMoreOverlaps = false;
            //}
            //float overlap_minbound_x = std::fabs(agents[agent_i].pos.x - m_minBoundaryX);
            //if (overlap_minbound_x < halfAgentSize) {
            //    agents[agent_i].pos.x += (halfAgentSize - overlap_minbound_x) * factor;
            //    noMoreOverlaps = false;
            //}
            //float overlap_maxbound_x = std::fabs(agents[agent_i].pos.x - m_maxBoundaryX);
            //if (overlap_maxbound_x < halfAgentSize) {
            //    agents[agent_i].pos.x -= (halfAgentSize - overlap_maxbound_x) * factor;
            //    noMoreOverlaps = false;
            //}
        }
        if (noMoreOverlaps) {
            return;
        }
    }
}

void TimeEvolution(std::vector<AgentStruct>& agents, ObjectStruct& object) {

    int numberOfAgents = agents.size();
    float dt = (float) 0.05;


    // evolve object's velocity
    float linearFriction = 0.95f;
    int numberOfObjectPoints = object.objectPoints.size();
    object.centerVel.x += object.centerAcc.x * dt;
    object.centerVel.y += object.centerAcc.y * dt;
    object.centerVel.x *= linearFriction;
    object.centerVel.y *= linearFriction;

    // evolve agents' position
    for (int agent_i = 0; agent_i < numberOfAgents; agent_i++) {
        agents[agent_i].pos.x +=  dt * agents[agent_i].vel.x;
        agents[agent_i].pos.y +=  dt * agents[agent_i].vel.y;
    }

    //evolve object's position
    object.centerPos.x += object.centerVel.x * dt;
    object.centerPos.y += object.centerVel.y * dt;
    for (int object_i = 0; object_i < numberOfObjectPoints; object_i++) {
        object.objectPoints[object_i].pos.x += object.centerVel.x * dt;
        object.objectPoints[object_i].pos.y += object.centerVel.y * dt;
    }
}

TrainingDataStruct ResetSwarmSimulation(const std::vector<float>& destinationxy, const std::vector<float>& objectxy) {

    TrainingDataStruct trainingData;
//    std::cout << "Reset swarm simulation!" << std::endl;
    m_destinationxy.resize(2);
    m_destinationxy[0] = destinationxy[0];
    m_destinationxy[1] = destinationxy[1];

    m_agents = InitializeAgent();
    m_object = InitializeObject(objectxy);
    m_oldDistanceToAgents = ComputeAgentObjectDistance();
    m_oldDistanceToDestination = std::sqrt(std::pow(m_object.centerPos.x - m_destinationxy[0],2) + std::pow(m_object.centerPos.y - m_destinationxy[1],2));
    WriteAllToFile(0);

    // prepare output for training dataset
    trainingData.reward = 0.0;
    trainingData.achieved = false;
    trainingData.stateOfSim = GetStateOfSimulation();
    return trainingData;
}

float GetAgentMaxSpeed() {
    return m_maxSpeed;
}

std::vector<float> GetStateOfSimulation() {
    std::vector<float> state;

    TwoDimensionStruct pushingPoint = GetGoodPushingPoint();
    for (const auto& agent : m_agents)
    {
        state.push_back(agent.pos.x);
        state.push_back(agent.pos.y);
        state.push_back(agent.vel.x);
        state.push_back(agent.vel.y);

        // object relative to this agent
        state.push_back(m_object.centerPos.x - agent.pos.x);
        state.push_back(m_object.centerPos.y - agent.pos.y);
        state.push_back(m_object.centerVel.x );
        state.push_back(m_object.centerVel.y );

        // target relative to object
        state.push_back(m_destinationxy[0] - m_object.centerPos.x);
        state.push_back(m_destinationxy[1] - m_object.centerPos.y);

        // 
    }

    return state;
}
float GetNumberOfAgents(){ 
    return m_agents.size(); 
}

void GetObjectBounds(float& minX, float& maxX, float& minY, float& maxY)
{
    minX =  1e9f;
    maxX = -1e9f;
    minY =  1e9f;
    maxY = -1e9f;

    for (const auto& p : m_object.objectPoints)
    {
        minX = std::min(minX, p.pos.x);
        maxX = std::max(maxX, p.pos.x);
        minY = std::min(minY, p.pos.y);
        maxY = std::max(maxY, p.pos.y);
    }
}
TwoDimensionStruct GetGoodPushingPoint() {
    TwoDimensionStruct p = m_object.centerPos;

    float dx = m_destinationxy[0] - m_object.centerPos.x;
    float dy = m_destinationxy[1] - m_object.centerPos.y;

    float len = std::sqrt(dx * dx + dy * dy);
    if (len < 1e-6f) {
        return p;
    }

    dx /= len;
    dy /= len;

    float minX, maxX, minY, maxY;
    GetObjectBounds(minX, maxX, minY, maxY);

    float halfWidth  = 0.5f * (maxX - minX);
    float halfHeight = 0.5f * (maxY - minY);

    float tx = 1e9f;
    float ty = 1e9f;

    if (std::fabs(dx) > 1e-6f) {
        tx = halfWidth / std::fabs(dx);
    }
    if (std::fabs(dy) > 1e-6f) {
        ty = halfHeight / std::fabs(dy);
    }
    float t = std::min(tx, ty);

    // opposite side of destination direction
    p.x = m_object.centerPos.x - dx * t;
    p.y = m_object.centerPos.y - dy * t;

    return p;
}
float ComputeAgentObjectDistance() {
    float total = 0.0f;

    TwoDimensionStruct pushingPoint = GetGoodPushingPoint();
    for (const auto& agent : m_agents) {
        float dx = agent.pos.x - pushingPoint.x;
        float dy = agent.pos.y - pushingPoint.y;


        total += std::sqrt(dx * dx + dy * dy) ;
    }
    return total / m_agents.size();
}

float ComputeObjectVelocityAlignment() {

    float dx = m_destinationxy[0] - m_object.centerPos.x ;
    float dy = m_destinationxy[1] - m_object.centerPos.y ;
    float len = std::sqrt(dx * dx + dy * dy);

    // ignore if agent is very close to the pushing point
    if (len < 0.02f) {
        return 0.0f;
    }
    dx /= len;
    dy /= len;

    // velocity must align with the pushing vector
    float vdot = m_object.centerVel.x * dx +  m_object.centerVel.y * dy;

    return vdot;
}

float ComputeAgentSpacingPenalty()
{
    float penalty = 0.0f;
    float minDist = 0.06f;

    for (size_t i = 0; i < m_agents.size(); i++) {
        for (size_t j = i + 1; j < m_agents.size(); j++) {
            float dx = m_agents[i].pos.x - m_agents[j].pos.x;
            float dy = m_agents[i].pos.y - m_agents[j].pos.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist < minDist) {
                penalty += (minDist - dist);
            }
        }
    }

    return penalty;
}

float ComputeReward(bool debug) {

    float newDistanceToDestination = std::sqrt(std::pow(m_object.centerPos.x - m_destinationxy[0], 2) + std::pow(m_object.centerPos.y - m_destinationxy[1], 2));
    float newDistanceToAgents = ComputeAgentObjectDistance();

    float stayCloseToDestinationReward = 0.1f * std::exp(-newDistanceToDestination * 3.0f);
    float stayCloseToPushingPointReward = 0.05f * std::exp(-newDistanceToAgents * 5.f); 
    float progressToPushingPointReward = 10 * (m_oldDistanceToAgents - newDistanceToAgents);
    float progressCloseToDestinationReward = 10 * (m_oldDistanceToDestination - newDistanceToDestination);


    //if (newDistanceToAgents < 0.06f) {
    //    progressToPushingPointReward = 0;
    //}

    float reward = progressCloseToDestinationReward +  progressToPushingPointReward ;

    if (IsObjectiveAchieved()) {
        reward += 10.0f;
    }


    // If close to destination, reward staying still
    if (IsObjectiveAchieved()) {
        // penalize object still moving near destination
        float objectSpeed = std::sqrt(m_object.centerVel.x * m_object.centerVel.x + m_object.centerVel.y * m_object.centerVel.y);
        reward -= 2.f * objectSpeed;

//        std::cout << -0.5f * objectSpeed << " ";
        //// small reward for being close
        //reward += 1.0f * (0.05f - newDistanceToDestination);
    }

    reward -= 0.001f; // small time penalty

    if (debug) {
        std::cout << progressCloseToDestinationReward << " "
            << stayCloseToDestinationReward << " "
            << newDistanceToDestination << " "
            << progressToPushingPointReward << " "
            << stayCloseToPushingPointReward << " "
            << newDistanceToAgents << " "
            << 0.5 * ComputeObjectVelocityAlignment() << "\n";
    }

    // reset them
    m_oldDistanceToDestination = newDistanceToDestination;
    m_oldDistanceToAgents = newDistanceToAgents;

    return reward;
}

float GetDistanceFromObjectToDestination() {
    return std::sqrt(std::pow(m_object.centerPos.x - m_destinationxy[0],2) + std::pow(m_object.centerPos.y - m_destinationxy[1],2));
}
bool IsObjectiveAchieved() {
    float distance = GetDistanceFromObjectToDestination();
    if (distance < 5e-2) {
        return true;
    }
    else {
        return false;
    }
}

void ApplyActionToAgents(const std::vector<float>& actions) {

    if (actions.size() != m_agents.size() * 2) {
        std::cout << "Action size mismatch! Expected "
            << m_agents.size() * 2
            << ", got "
            << actions.size()
            << std::endl;
        return;
    }

    for (size_t i = 0; i < m_agents.size(); i++) {
        float vx = actions[2 * i];
        float vy = actions[2 * i + 1];

        if (vx < -m_maxSpeed) {
            m_agents[i].vel.x = -m_maxSpeed ;
        }
        else if (vx > m_maxSpeed) {
            m_agents[i].vel.x = m_maxSpeed;
        }
        else {
            m_agents[i].vel.x = vx;
        }

        if (vy < -m_maxSpeed) {
            m_agents[i].vel.y = -m_maxSpeed ;
        }
        else if (vy > m_maxSpeed) {
            m_agents[i].vel.y = m_maxSpeed;
        }
        else {
            m_agents[i].vel.y = vy;
        }
    }



}

TrainingDataStruct EvolveSwarmSimulation( bool debug ) {

    TrainingDataStruct trainingData;
    if (m_agents.empty()) {
        std::cout << " There is no agents to evolve !! \n";
        return trainingData;
    }

//    std::cout << "Evolve swarm simulation!" << std::endl;


    TimeEvolution(m_agents, m_object);
    ApplyConstraints(m_agents, m_object);

    // prepare output for training dataset
    trainingData.reward = ComputeReward(debug);
    trainingData.achieved = IsObjectiveAchieved();
    trainingData.stateOfSim = GetStateOfSimulation();

    return trainingData;
}
 