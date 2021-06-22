#ifndef MCM_OUTPUT_H
#define MCM_OUTPUT_H

#include "UI/mcm.h"

class MCM_Output
{
public:
    MCM_Output();
    static double Integrin(parameters * para, iteration * iter);

    static int Bounded_receptor(parameters * para, iteration * iter);

    static double Effective_spring(parameters * para, iteration * iter);

    static void Cycle_number(iteration * iter);

    static void Time_variables(parameters * para, iteration * iter, double Results[][8], double AV_results[], int ncyc);

    static void Average_variables(parameters * para, iteration * iter, double AV_results[]);
};

#endif // MCM_OUTPUT_H
