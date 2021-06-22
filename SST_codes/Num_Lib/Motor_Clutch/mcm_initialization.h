#ifndef MCM_INITIALIZATION_H
#define MCM_INITIALIZATION_H

#include "UI/mcm.h"
#include "ui_mcm.h"

class MCM_Initialization
{
public:
    MCM_Initialization();

    static iteration Iteration(parameters * para);

    static void Parameters(parameters * para, Ui::MCM * ui, int type_number, int num_links, int simulation_cycle);
};

#endif // MCM_INITIALIZATION_H
