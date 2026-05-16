#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "SwarmSim.h"

// python binder
PYBIND11_MODULE(swarm_simulation_py, m) {
    pybind11::class_<TrainingDataStruct>(m, "TrainingDataStruct")
        .def_readonly("reward", &TrainingDataStruct::reward)
        .def_readonly("achieved", &TrainingDataStruct::achieved)
        .def_readonly("stateOfSim", &TrainingDataStruct::stateOfSim);

    m.def("Evolve", &EvolveSwarmSimulation, "Run the swarm simulation");
    m.def("Reset", &ResetSwarmSimulation, "Reset the swarm simulation");
    m.def("ApplyAction", &ApplyActionToAgents);
    m.def("WriteAll", &WriteAllToFile, "Ouput agents and object phase spase");
    m.def("GetNumberOfAgents", &GetNumberOfAgents, "Get number of agents");
    m.def("GetAgentMaxSpeed", &GetAgentMaxSpeed, "Get max speed of agents");
    m.def("GetDistanceFromObjectToDestination", &GetDistanceFromObjectToDestination, "Get distance between object and destination");
}