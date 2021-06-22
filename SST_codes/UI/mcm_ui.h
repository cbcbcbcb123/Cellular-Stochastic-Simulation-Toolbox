#ifndef MCM_UI_H
#define MCM_UI_H

// MUST inlude "mcm.h" "ui_mcm.h"

#include "mcm.h"
#include "ui_mcm.h"
#include "Num_Lib/Motor_Clutch/mainloop.h"
#include "QCustomPlot/qcustomplot.h"
#include <QTextBrowser>

#include <iostream>
#include <fstream>
#include <cmath>
#include <QWidget>
#include <QDialog>
#include <QTableWidget>
#include <QString>
#include <QTextEdit>
#include <QMdiArea>
#include <QTextBrowser>
#include <limits>
#include <random>
#include <vector>

class MCM_UI
{
public:
    MCM_UI();

    static void Set_UI_Main(Ui::MCM * ui, QCustomPlot * customPlot, QTextBrowser * MC_results);

    static void Set_UI_Parameters(Ui::MCM * ui, int type_number);

    static void Plot_1(int K, QCustomPlot * customPlot, QVector<double> x1, QVector<double> x2, QVector<double> x3, QVector<double> x4, QVector<double> x8);

    static void Save_Data_1(parameters * para, QVector<double> x1, QVector<double> x2, QVector<double> x3, QVector<double> x4, QVector<double> x8);

    static void Plot_2(int K, QCustomPlot * customPlot, QVector<double> a0, QVector<double> a1, QVector<double> a2, QVector<double> a3, QVector<double> a6);

    static void Save_Data_2(QVector<double> * a1, QVector<double> * a2, QVector<double> * a3, QVector<double> * a4, QVector<double> * a5, QVector<double> * a6, double value_up, double value_down, double dvalue);
};

#endif // MCM_UI_H
