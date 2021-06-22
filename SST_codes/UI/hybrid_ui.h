#ifndef HYBRID_UI_H
#define HYBRID_UI_H

// MUST inlude "hybrid.h" "ui_hybrid.h"
#include "Num_Lib/CRS/parameters.h"
#include "Num_Lib/CRS/reaction.h"
#include "Num_Lib/CRS/iteration.h"
#include "hybrid.h"
#include "ui_hybrid.h"
#include "QCustomPlot/qcustomplot.h"
// All Check
#include <QMessageBox>
#include <QDialog>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <QFileDialog>
#include <QAxObject>
#include <QVariant>
#include <QString>

class Hybrid_UI
{
public:
    Hybrid_UI();

    static void Set_UI_Main(Ui::Hybrid * ui, QCustomPlot * customPlot);

    static void Set_UI_Reactant_Table(Ui::Hybrid * ui, int Reactant_number, bool From_excel, QList<QList<QVariant>> excel_model);

    static void Set_UI_Reaction_Table(Ui::Hybrid * ui, int Reactant_number, int Reaction_channles, bool From_excel, QList<QList<QVariant>> excel_model);

    static void ExportData(int Reactant_number, Ui::Hybrid * ui, parameters_CRS * para_hyb, double * t, double ** array);

    static void Plot(int K, QCustomPlot * customPlot, int count, double * t, double ** array);
};

#endif // HYBRID_UI_H
