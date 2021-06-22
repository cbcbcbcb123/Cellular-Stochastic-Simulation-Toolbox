#ifndef SSA_SOLVER_H
#define SSA_SOLVER_H

#include "Num_Lib/CRS/parameters.h"
#include "Num_Lib/CRS/reaction.h"
#include "Num_Lib/CRS/iteration.h"
#include "Num_Lib/Basic_Class/basic_class.h"
#include "UI/hybrid.h"
#include "ui_hybrid.h"
#include <QVariant>

class SSA_Solver
{
public:
    SSA_Solver();

    static void DM(Ui::Hybrid * ui, QVector<struct reaction> * reaction_collection_hyb, iteration_CRS * iter_hyb, parameters_CRS * para_hyb, int Reactant_number, double * t, double ** array);

    static void RSSA(Ui::Hybrid * ui, QVector<struct reaction> * reaction_collection_hyb, iteration_CRS * iter_hyb, parameters_CRS * para_hyb, int Reactant_number, double * t, double ** array, int row_count, QList<QList<QVariant>> * excel_list);

    static void ODE_DM(Ui::Hybrid * ui, QVector<struct reaction> * reaction_collection_hyb, iteration_CRS * iter_hyb, parameters_CRS * para_hyb, int Reactant_number, double * t, double ** array);

    static void Update_amu(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection);

    static void Reaction_Time_DM(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection);

    static double Reaction_Time_DM_ODE(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection, double y_pre[], double a0_t);

    static void Update_amu_RSSA(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection);

    static void Reaction_Time_RSSA(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection);

    static void Update_amu_tRSSA(parameters_CRS * para, iteration_CRS * iter, QVector<reaction> * reaction_collection, int row_count, QList<QList<QVariant>> * excel_list);
};

#endif // SSA_SOLVER_H
