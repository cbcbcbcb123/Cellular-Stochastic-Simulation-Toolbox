#ifndef CRS_OUTPUT_H
#define CRS_OUTPUT_H

#include "Num_Lib/CRS/parameters.h"
#include "Num_Lib/CRS/reaction.h"
#include "Num_Lib/CRS/iteration.h"
#include "UI/hybrid.h"
#include "ui_hybrid.h"

class CRS_Output
{
public:
    CRS_Output();

    static void Initialization(parameters_CRS * para_hyb, int Reactant_number, double * t, double ** array);
};

#endif // CRS_OUTPUT_H
