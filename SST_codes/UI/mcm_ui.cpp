#include "UI/mcm_ui.h"

MCM_UI::MCM_UI()
{

}

void MCM_UI::Set_UI_Main(Ui::MCM * ui, QCustomPlot * customPlot, QTextBrowser * MC_results)
{
    // Set plot window
    ui->mdiArea_plot->addSubWindow(customPlot);
    customPlot->showMaximized(); // "showMaximized()" must to go to the next line of "addSubWindow()"
    customPlot->xAxis->setVisible(true);
    customPlot->yAxis->setVisible(true);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->replot();

    // Set output window
    ui->mdiArea_output->addSubWindow(MC_results);
    MC_results->showMaximized();
}

void MCM_UI::Set_UI_Parameters(Ui::MCM * ui, int type_number) // Note it is Ui::MCM * ui, but not MCM * ui !!!
{
    // Set parameter tables
    ui->tableWidget_1->setColumnCount(type_number + 1);
    ui->tableWidget_2->setColumnCount(type_number + 1);
    ui->tableWidget_3->setColumnCount(type_number + 1);
    ui->tableWidget_4->setColumnCount(type_number + 1);

    QStringList sListHeader_1; sListHeader_1 << "Parameters";
    QStringList sListHeader_2; sListHeader_2 << "Parameters";
    QStringList sListHeader_3; sListHeader_3 << "Parameters";
    QStringList sListHeader_4; sListHeader_4 << "Parameters";
    QStringList sListHeader_5; sListHeader_5 << "Parameters" << "E1" << "E2" << "R1" << "R2";
    QStringList sListHeader_6; sListHeader_6 << "Parameters" << "Value";
    QStringList sListHeader_7; sListHeader_7 << "Parameters" << "Value";

    for (int i = 0; i < type_number; i++)
    {
        sListHeader_1 << "Type " + QString::number(i+1, 10);
        sListHeader_2 << "Type " + QString::number(i+1, 10);
        sListHeader_3 << "Type " + QString::number(i+1, 10);
        sListHeader_4 << "Type " + QString::number(i+1, 10);
    }

    ui->tableWidget_1->setHorizontalHeaderLabels(sListHeader_1);
    ui->tableWidget_2->setHorizontalHeaderLabels(sListHeader_2);
    ui->tableWidget_3->setHorizontalHeaderLabels(sListHeader_3);
    ui->tableWidget_4->setHorizontalHeaderLabels(sListHeader_4);
    ui->tableWidget_5->setHorizontalHeaderLabels(sListHeader_5);
    ui->tableWidget_6->setHorizontalHeaderLabels(sListHeader_6);
    ui->tableWidget_7->setHorizontalHeaderLabels(sListHeader_7);

    ui->tableWidget_1->setItem(0,0,new QTableWidgetItem("Spring stiffness (pN/nm)"));
    ui->tableWidget_1->setItem(1,0,new QTableWidgetItem("Base membrane receptor density"));
    ui->tableWidget_1->setItem(2,0,new QTableWidgetItem("On rate (1/s)"));
    ui->tableWidget_1->setItem(3,0,new QTableWidgetItem("Bond type [1-slip bond 2-catch bond]"));

    ui->tableWidget_2->setItem(0,0,new QTableWidgetItem("Off rate (1/s)"));
    ui->tableWidget_2->setItem(1,0,new QTableWidgetItem("Rupture force (pN)"));

    ui->tableWidget_3->setItem(0,0,new QTableWidgetItem("Off rate I (1/s)"));
    ui->tableWidget_3->setItem(1,0,new QTableWidgetItem("Off rate II (1/s)"));
    ui->tableWidget_3->setItem(2,0,new QTableWidgetItem("Rupture force I (pN)"));
    ui->tableWidget_3->setItem(3,0,new QTableWidgetItem("Rupture force II (pN)"));

    ui->tableWidget_4->setItem(0,0,new QTableWidgetItem("Threshold tension (pN)"));
    ui->tableWidget_4->setItem(1,0,new QTableWidgetItem("Increased density"));
    ui->tableWidget_4->setItem(2,0,new QTableWidgetItem("Minimal density"));
    ui->tableWidget_4->setItem(3,0,new QTableWidgetItem("Maximal density"));

    ui->tableWidget_5->setItem(0,0,new QTableWidgetItem("Value"));

    ui->tableWidget_6->setItem(0,0,new QTableWidgetItem("Actin flow (nm/s)"));
    ui->tableWidget_6->setItem(1,0,new QTableWidgetItem("Base myosin density"));
    ui->tableWidget_6->setItem(2,0,new QTableWidgetItem("Stall force (pN)"));

    ui->tableWidget_7->setItem(0,0,new QTableWidgetItem("Threshold tension (pN)"));
    ui->tableWidget_7->setItem(1,0,new QTableWidgetItem("Added density"));
    ui->tableWidget_7->setItem(2,0,new QTableWidgetItem("Minimal density"));
    ui->tableWidget_7->setItem(3,0,new QTableWidgetItem("Maximal density"));


    // Set default value
    for (int i = 1; i < type_number+1; i++) // Note from i = 1 on...
    {
        ui->tableWidget_1->setItem(0,i,new QTableWidgetItem("0.8"));
        ui->tableWidget_1->setItem(1,i,new QTableWidgetItem("300"));
        ui->tableWidget_1->setItem(2,i,new QTableWidgetItem("0.001"));
        ui->tableWidget_1->setItem(3,i,new QTableWidgetItem("1"));

        ui->tableWidget_2->setItem(0,i,new QTableWidgetItem("0.1"));
        ui->tableWidget_2->setItem(1,i,new QTableWidgetItem("2"));

        ui->tableWidget_3->setItem(0,i,new QTableWidgetItem("0.4"));
        ui->tableWidget_3->setItem(1,i,new QTableWidgetItem("10"));
        ui->tableWidget_3->setItem(2,i,new QTableWidgetItem("8"));
        ui->tableWidget_3->setItem(3,i,new QTableWidgetItem("8"));

        ui->tableWidget_4->setItem(0,i,new QTableWidgetItem("4"));
        ui->tableWidget_4->setItem(1,i,new QTableWidgetItem("0"));
        ui->tableWidget_4->setItem(2,i,new QTableWidgetItem("300"));
        ui->tableWidget_4->setItem(3,i,new QTableWidgetItem("10000"));
    }

    ui->tableWidget_5->setItem(0,1,new QTableWidgetItem("1"));
    ui->tableWidget_5->setItem(0,2,new QTableWidgetItem("1"));
    ui->tableWidget_5->setItem(0,3,new QTableWidgetItem("1"));
    ui->tableWidget_5->setItem(0,4,new QTableWidgetItem("1"));

    ui->tableWidget_6->setItem(0,1,new QTableWidgetItem("120"));
    ui->tableWidget_6->setItem(1,1,new QTableWidgetItem("50"));
    ui->tableWidget_6->setItem(2,1,new QTableWidgetItem("2"));

    ui->tableWidget_7->setItem(0,1,new QTableWidgetItem("2"));
    ui->tableWidget_7->setItem(1,1,new QTableWidgetItem("0"));
    ui->tableWidget_7->setItem(2,1,new QTableWidgetItem("50"));
    ui->tableWidget_7->setItem(3,1,new QTableWidgetItem("500"));


    // Optimize tables
    ui->tableWidget_1->resizeColumnsToContents();
    ui->tableWidget_1->resizeRowsToContents();
    ui->tableWidget_2->resizeColumnsToContents();
    ui->tableWidget_2->resizeRowsToContents();
    ui->tableWidget_3->resizeColumnsToContents();
    ui->tableWidget_3->resizeRowsToContents();
    ui->tableWidget_4->resizeColumnsToContents();
    ui->tableWidget_4->resizeRowsToContents();
    ui->tableWidget_5->resizeColumnsToContents();
    ui->tableWidget_5->resizeRowsToContents();
    ui->tableWidget_6->resizeColumnsToContents();
    ui->tableWidget_6->resizeRowsToContents();
    ui->tableWidget_7->resizeColumnsToContents();
    ui->tableWidget_7->resizeRowsToContents();

    ui->tableWidget_1->verticalHeader()->hide();
    ui->tableWidget_2->verticalHeader()->hide();
    ui->tableWidget_3->verticalHeader()->hide();
    ui->tableWidget_4->verticalHeader()->hide();
    ui->tableWidget_5->verticalHeader()->hide();
    ui->tableWidget_6->verticalHeader()->hide();
    ui->tableWidget_7->verticalHeader()->hide();
}

void MCM_UI::Plot_1(int K, QCustomPlot * customPlot, QVector<double> x1, QVector<double> x2, QVector<double> x3, QVector<double> x4, QVector<double> x8)
{
    customPlot->clearGraphs();
    customPlot->addGraph();

    if (K==0)
    {
        customPlot->graph(0)->setData(x1, x4);
        customPlot->xAxis->setLabel("Time (s)");
        customPlot->yAxis->setLabel("Adhesion density");
    }
    else if (K==1)
    {
        customPlot->graph(0)->setData(x1, x2);
        customPlot->xAxis->setLabel("Time (s)");
        customPlot->yAxis->setLabel("Actin flow (nm/s)");
    }
    else if (K==2)
    {
        customPlot->graph(0)->setData(x1, x3);
        customPlot->xAxis->setLabel("Time (s)");
        customPlot->yAxis->setLabel("Force (pN)");
    }
    else if (K==3)
    {
        customPlot->graph(0)->setData(x1, x8);
        customPlot->xAxis->setLabel("Time (s)");
        customPlot->yAxis->setLabel("Active site density");
    }

    customPlot->rescaleAxes();
    customPlot->replot();
}

void MCM_UI::Save_Data_1(parameters * para, QVector<double> x1, QVector<double> x2, QVector<double> x3, QVector<double> x4, QVector<double>x8)
{
    std::ofstream dataFile_Time;
    std::ofstream dataFile_Flow;
    std::ofstream dataFile_Force;
    std::ofstream dataFile_Adhesion;
    std::ofstream dataFile_Active_sites;

    QString file_path = QFileDialog::getExistingDirectory(NULL,"请选择结果保存路径...","./");

    dataFile_Time.open((file_path+"/Time"+".txt").toStdString());
    dataFile_Flow.open((file_path+"/Flow"+".txt").toStdString());
    dataFile_Force.open((file_path+"/Force"+".txt").toStdString());
    dataFile_Adhesion.open((file_path+"/Adhesion"+".txt").toStdString());
    dataFile_Active_sites.open((file_path+"/Active_sites"+".txt").toStdString());

    for (int i = 0; i < (*para).Ncyc; i++)
    {
        dataFile_Time << x1[i] << std::endl;
        dataFile_Force << x2[i] << std::endl;
        dataFile_Flow << x3[i] << std::endl;
        dataFile_Adhesion << x4[i] << std::endl;
        dataFile_Active_sites << x8[i] << std::endl;
    }

    dataFile_Time.close();
    dataFile_Flow.close();
    dataFile_Force.close();
    dataFile_Adhesion.close();
    dataFile_Active_sites.close();
}

void MCM_UI::Plot_2(int K, QCustomPlot * customPlot, QVector<double> a0, QVector<double> a1, QVector<double> a2, QVector<double> a3, QVector<double> a6)
{
    customPlot->clearGraphs();
    customPlot->addGraph();

    if (K==0)
    {
        customPlot->graph(0)->setData(a0, a3);
        customPlot->xAxis->setLabel("Mechanics log10");
        customPlot->yAxis->setLabel("Adhesion density");
    }
    else if (K==1)
    {
        customPlot->graph(0)->setData(a0, a1);
        customPlot->xAxis->setLabel("Mechanics log10");
        customPlot->yAxis->setLabel("Force (pN)");
    }
    else if (K==2)
    {
        customPlot->graph(0)->setData(a0, a2);
        customPlot->xAxis->setLabel("Mechanics log10");
        customPlot->yAxis->setLabel("Actin flow (nm/s)");
    }
    else if (K==3)
    {
        customPlot->graph(0)->setData(a0, a6);
        customPlot->xAxis->setLabel("Mechanics log10");
        customPlot->yAxis->setLabel("Active sites density");
    }

    customPlot->rescaleAxes();
    customPlot->replot();
}

void MCM_UI::Save_Data_2(QVector<double> * a1, QVector<double> * a2, QVector<double> * a3, QVector<double> * a4, QVector<double> * a5, QVector<double> * a6, double value_up, double value_down, double dvalue)
{
    std::ofstream dataFile_Flow;
    std::ofstream dataFile_Force;
    std::ofstream dataFile_Adhesion;
    std::ofstream dataFile_Ensemble_stiffness;
    std::ofstream dataFile_Cycle_time;
    std::ofstream dataFile_Active_sites;

    QString file_path = QFileDialog::getExistingDirectory(NULL,"请选择结果保存路径...","./");

    dataFile_Flow.open((file_path+"/Flow"+".txt").toStdString());
    dataFile_Force.open((file_path+"/Force"+".txt").toStdString());
    dataFile_Adhesion.open((file_path+"/Adhesion"+".txt").toStdString());
    dataFile_Ensemble_stiffness.open((file_path+"/Ensemble_stiffness"+".txt").toStdString());
    dataFile_Cycle_time.open((file_path+"/Cycle_time"+".txt").toStdString());
    dataFile_Active_sites.open((file_path+"/Active_sites"+".txt").toStdString());

    int k = 0;
    double dv = (value_up-value_down)/dvalue;
    for (double i = value_down; i <= value_up; i = i+dv)
    {
        dataFile_Force << (*a1)[k] << std::endl;
        dataFile_Flow << (*a2)[k]<< std::endl;
        dataFile_Adhesion << (*a3)[k] << std::endl;
        dataFile_Ensemble_stiffness << (*a4)[k] << std::endl;
        dataFile_Cycle_time << (*a5)[k] << std::endl;
        dataFile_Active_sites << (*a6)[k] << std::endl;
        k = k+1;
    }

    dataFile_Flow.close();
    dataFile_Force.close();
    dataFile_Adhesion.close();
    dataFile_Ensemble_stiffness.close();
    dataFile_Cycle_time.close();
    dataFile_Active_sites.close();
}
