#ifndef HYBRID_H
#define HYBRID_H

#include "QCustomPlot/qcustomplot.h"
#include "UI/hybrid_ui.h"
#include "Num_Lib/CRS/parameters.h"
#include "Num_Lib/CRS/reaction.h"
#include "Num_Lib/CRS/iteration.h"
#include "Num_Lib/CRS/ode_solver.h"
#include "Num_Lib/CRS/ssa_solver.h"
#include "Num_Lib/CRS/crs_initialization.h"
#include "Num_Lib/CRS/crs_output.h"
// All Check
#include <QDialog>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <QAxObject>
#include <QVariant>

namespace Ui {
class Hybrid;
}

class Hybrid : public QDialog
{
    Q_OBJECT

public:
    explicit Hybrid(QWidget *parent = nullptr);

    ~Hybrid();

private slots:
    void on_pushButton_Ini_tables_clicked();

    void on_pushButton_Set_Reaction_clicked();

    void on_pushButton_Check_clicked();

    void on_pushButton_Run_clicked();

    void on_pushButton_Plot_clicked();

    void on_pushButton_Output_clicked();

    void on_pushButton_Clear_clicked();

    void Import_Time_Dependent_Rate();

    void on_pushButton_SBML_File_clicked();

private:
    Ui::Hybrid *ui;

private:
    // #### MSUT ADD class into QVector<class reaction>, here is struct
    QVector<struct reaction> * reaction_collection_hyb;
    iteration_CRS iter_hyb;
    parameters_CRS para_hyb;

    // Import model
    QList<QList<QVariant>> excel_model;
    int row_count_model; int column_count_model;
    bool From_excel = false;

    // Reaction paremeters
    int Reactant_number = 0;
    int Reaction_channles = 0;

    // Import time-dependent rate
    QList<QList<QVariant>> excel_list;
    int row_count;

    // Check all processes
    bool Check_1 = false, Check_2 = false, Check_3 = false, Check_4 = true;

    // Import data
    int count = 0;
    double * t;
    double ** array;

    // Plot
    QCustomPlot * customPlot;
};

#endif // HYBRID_H
