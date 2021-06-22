#ifndef REACTION_H
#define REACTION_H

#include <QVector>

struct reaction
{
    QVector<int> SS; // Reactant stoichiometric coefficient
    double rate; // Chemical reaction rate
    QVector<int> vmu; // State change vector
    double hmu; // Combination Vector
    double amu; // Propensity function vector
    int D_S; // 0 for fast reaction (ODE or tODE) and 1 for low eaction (DM or RSSA)
    double amu_L; // Low value of propensity function vector in RSSA
    double amu_H; // High value of propensity function vector in RSSA
    double hmu_L; // Low value of propensity function vector in RSSA
    double hmu_H; // High value of propensity function vector in RSSA
};


#endif
