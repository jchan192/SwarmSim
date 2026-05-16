#pragma once
#include <vector>
#include <string>

struct TwoDimensionStruct {
    float x;
    float y;
};

struct AgentStruct {
    int id;
    float m;
    TwoDimensionStruct pos;
    TwoDimensionStruct vel;
    TwoDimensionStruct acc;
};

struct ObjectStruct {
    float M;
    TwoDimensionStruct centerPos;
    TwoDimensionStruct centerVel;
    TwoDimensionStruct centerAcc;
    std::vector<AgentStruct> objectPoints ;
};

struct TrainingDataStruct {
    float reward;
    bool achieved;
    std::vector<float> stateOfSim;
};

extern std::string m_outputDir;
extern float m_distanceBetweenAgents;
extern float m_minBoundaryX;
extern float m_maxBoundaryX;
extern float m_minBoundaryY;
extern float m_maxBoundaryY;
extern float m_agentSize;
extern float m_maxSpeed;
extern float m_oldDistanceToDestination;
extern float m_oldDistanceToAgents;
extern int m_totalTimeSteps;

extern std::vector<float> m_destinationxy;
extern std::vector<AgentStruct> m_agents;
extern ObjectStruct m_object;

ObjectStruct InitializeObject(const std::vector<float>& objectxy);
std::vector<AgentStruct> InitializeAgent();
TrainingDataStruct ResetSwarmSimulation(const std::vector<float>& destinationxy, const std::vector<float>& objectxy);
TrainingDataStruct EvolveSwarmSimulation(bool debug=false);
void ApplyActionToAgents(const std::vector<float>& actions);
void WriteAgentsToFile(const std::vector<AgentStruct>& agents, const int timeStep);
void WriteObjectToFile(ObjectStruct object, const int timeStep);
void WriteAllToFile(const int timeStep);
void ComputeAcceleration(std::vector<AgentStruct>& agents, ObjectStruct& object);
void ApplyConstraints(std::vector<AgentStruct>& agents, ObjectStruct& object);
void TimeEvolution(std::vector<AgentStruct>& agents, ObjectStruct& object);
bool IsObjectiveAchieved();
float ComputeReward(bool debug=false);
float ComputeAgentObjectDistance();
float GetNumberOfAgents();
float GetAgentMaxSpeed();
float GetDistanceFromObjectToDestination();
void GetObjectBounds(float& minX, float& maxX, float& minY, float& maxY);
TwoDimensionStruct GetGoodPushingPoint();
std::vector<float> GetStateOfSimulation();

