#include "UI/mainwindow.h"
#include "ui_mainwindow.h"

#include "UI/mcm.h"
#include "ui_mcm.h"

#include "UI/hybrid.h"
#include "ui_hybrid.h"

#include "UI/cpm.h"
#include "ui_cpm.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set Figure
    QPixmap p1, p2;
    p1.load(":/MCM.jpg");
    ui->label_2->setPixmap(p1);
    ui->label_2->setScaledContents(true);
    ui->label_2->show();
    p2.load(":/CRS.jpg");
    ui->label_3->setPixmap(p2);
    ui->label_3->setScaledContents(true);
    ui->label_3->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionHybrid_Algorithm_HA_triggered()
{
    // 0.Set central widget
    QMdiArea * mdiArea_CRS = new QMdiArea(this);
    this->setCentralWidget(mdiArea_CRS);
    mdiArea_CRS->show();
    // 1.Open CRS interface
    Hybrid * dialog = new Hybrid(this);
    mdiArea_CRS->addSubWindow(dialog);
    dialog->showMaximized();
    dialog->show();
}

void MainWindow::on_actionMolecular_Clutch_Model_MCM_2_triggered()
{
    // 0.Set central widget
    QMdiArea * mdiArea_MCM = new QMdiArea(this);
    this->setCentralWidget(mdiArea_MCM);
    mdiArea_MCM->show();
    // 1.Open MCM interface
    MCM * dialog = new MCM(this);
    mdiArea_MCM->addSubWindow(dialog);
    dialog->showMaximized();
    dialog->show();
}

void MainWindow::on_actionCell_Potts_Model_CPM_2_triggered()
{
    // 0.Set central widget
    QMdiArea * mdiArea_CPM = new QMdiArea(this);
    this->setCentralWidget(mdiArea_CPM);
    mdiArea_CPM->show();
    // 1.Open CPM interface
    CPM * dialog = new CPM(this);
    mdiArea_CPM->addSubWindow(dialog);
    dialog->showMaximized();
    dialog->show();
}

void MainWindow::on_pushButton_clicked()
{
    // Open MCM interface
    MainWindow::on_actionMolecular_Clutch_Model_MCM_2_triggered();
}

void MainWindow::on_pushButton_2_clicked()
{
    // Open CRS interface
    MainWindow::on_actionHybrid_Algorithm_HA_triggered();
}
