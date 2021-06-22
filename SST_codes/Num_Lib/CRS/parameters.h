#ifndef PARAMETERS_SSA_H
#define PARAMETERS_SSA_H

#include <string>

struct parameters_CRS
{
    int N_r; // Reactant number
    int M_r; // Reaction number
    int Ncyc; // Simulation time
    std::string Methods; // Simulation methods
    double t; // Reaction time
    double h; // Step size for ODE method
    double eps; // Error in ODE method
};

#endif
