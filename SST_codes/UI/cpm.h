#ifndef CPM_H
#define CPM_H

#include <QDialog>
#include <QPainter>
#include <QTimer>
#include <cmath>
#include "QCustomPlot/qcustomplot.h"
#include "Num_Lib/CPM/initialization_cell.h"
#include "Num_Lib/CPM/find_neighborhood.h"
#include "Num_Lib/CPM/hamiltonian.h"
#include "Num_Lib/CPM/plot_cpm.h"

namespace Ui {
class CPM;
}

class CPM : public QDialog
{
    Q_OBJECT

public:
    explicit CPM(QWidget *parent = nullptr);
    ~CPM();

private slots:

    void on_pushButton_Set_Background_clicked();

    void Main_Cycle();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::CPM *ui;

    int x_pix;
    int y_pix;
    int x_loc;
    int y_loc;
    int N;

    int loc_x;
    int loc_y;
    int R;

    int Sim_time;

    QVector <int> Target_Volume;
    QVector <int> Target_S;
    QVector <QColor> qc;

    double T;
    QVector<QVector <double>> J;

    int tem_aa = 0;
    int tem_bb = 0;
    int aa = 0;
    int bb = 0;
    int temple_id = 0;
    int count = 0;
    double adhesion = 0.0;

    QVector <double> lamda_V;
    QVector <double> lamda_B;
    QVector <double> lamda_Chem_Force;

    double derta_V = 0.0;
    double derta_B = 0.0;
    double derta_Adh = 0.0;
    double derta_Chem_Force = 0.0;
    double derta_H = 0.0;
    double derta_P = 0.0;

    int Boader_add_num = 0;

    QTimer * timer;

    QImage * img;
    QPixmap pixmap;
    QVector<QVector <int>> array;
    QVector<QVector <int>> Boader;

};

#endif // CPM_H
