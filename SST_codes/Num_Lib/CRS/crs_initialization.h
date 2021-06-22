#ifndef CRS_INITIALIZATION_H
#define CRS_INITIALIZATION_H

#include "Num_Lib/CRS/parameters.h"
#include "Num_Lib/CRS/reaction.h"
#include "Num_Lib/CRS/iteration.h"
#include "Num_Lib/CRS/ssa_solver.h"
#include "UI/hybrid.h"
#include "ui_hybrid.h"

class CRS_Initialization
{
public:
    CRS_Initialization();

    static void Parameters(iteration_CRS * iter_hyb, parameters_CRS * para_hyb);

    static bool Reactions(QVector<struct reaction> * reaction_collection_hyb, iteration_CRS * iter_hyb, parameters_CRS * para_hyb, Ui::Hybrid * ui, int Reactant_number, int Reaction_channles, bool Check_3);

    static void Methods_and_classifier(QVector<struct reaction> * reaction_collection_hyb, iteration_CRS * iter_hyb, parameters_CRS * para_hyb, Ui::Hybrid * ui, int Reaction_channles);
};

#endif // CRS_INITIALIZATION_H
