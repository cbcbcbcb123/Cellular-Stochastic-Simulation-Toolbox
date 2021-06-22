#ifndef ODE_SOLVER_H
#define ODE_SOLVER_H

#include "Num_Lib/CRS/parameters.h"
#include "Num_Lib/CRS/reaction.h"
#include "Num_Lib/CRS/iteration.h"
#include <QVariant>
#include "UI/hybrid.h"
#include "ui_hybrid.h"

class ODE_Solver
{
public:
    ODE_Solver();

    static void Time_Independent_ODE(Ui::Hybrid * ui, QVector<struct reaction> * reaction_collection_hyb, iteration_CRS * iter_hyb, parameters_CRS * para_hyb, int Reactant_number, double * t, double ** array);

    static void Time_Dependent_ODE(Ui::Hybrid * ui, QList<QList<QVariant>> * excel_list, int row_count, QVector<struct reaction> * reaction_collection_hyb, iteration_CRS * iter_hyb, parameters_CRS * para_hyb, int Reactant_number, double * t, double ** array);

    static void Euler(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection);

    static void Runge_Kutta(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection);

    static void Treanor(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection);

    static void Update_amu_ODE(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection, double * d);

};

#endif // ODE_SOLVER_H
