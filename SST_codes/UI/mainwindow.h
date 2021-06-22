#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include "QCustomPlot/qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public:
    Ui::MainWindow * ui;

private slots:
    void on_actionHybrid_Algorithm_HA_triggered();

    void on_actionMolecular_Clutch_Model_MCM_2_triggered();

    void on_actionCell_Potts_Model_CPM_2_triggered();

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H
