#ifndef MCM_H
#define MCM_H

#include "Num_Lib/Motor_Clutch/parameters.h"
#include "Num_Lib/Motor_Clutch/iteration.h"
#include "QCustomPlot/qcustomplot.h"
#include "Num_Lib/Motor_Clutch/mainloop.h"
#include "UI/mcm_ui.h"

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

namespace Ui {
class MCM;
}

class MCM : public QWidget
{
    Q_OBJECT

public:
    explicit MCM(QWidget *parent = nullptr);
    ~MCM();

    // Main window
    QCustomPlot * customPlot; // Plot window
    QTextBrowser * MC_results; // Output window

    // Set parameters
    int type_number = 1;
    int num_links = 50;
    int simulation_cycle = 50000;
    parameters para;

    // Result container
    QVector<double> x1, x2, x3, x4, x5, x6, x7, x8;
    QVector<double> a0, a1, a2, a3, a4, a5, a6;

    // Sensitivity
    int Para_mechanosen_ID = 0; // 0 for E1; 1 for E2; 2 for R1; 3 for R2
    double value_up = 2, value_down = -2, dvalue = 20; // Parameter range

private slots:
    void on_pushButton_para_clicked();

    void on_pushButton_run_clicked();

    void on_pushButton_plot_1_clicked();

    void on_pushButton_save_1_clicked();

    void on_pushButton_analysis_clicked();

    void on_pushButton_plot_2_clicked();

    void on_pushButton_save_2_clicked();

private:
    Ui::MCM * ui;
};

#endif // MCM_H
