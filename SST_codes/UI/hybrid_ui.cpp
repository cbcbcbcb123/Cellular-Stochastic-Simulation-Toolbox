#include "hybrid_ui.h"

Hybrid_UI::Hybrid_UI()
{

}

void Hybrid_UI::Set_UI_Main(Ui::Hybrid * ui, QCustomPlot * customPlot)
{
    // Set plot subwindow
    ui->mdiArea_plot->addSubWindow(customPlot);
    // ->showMaximized() must be placed on the next line of ->addSubWindow()
    customPlot->showMaximized();
    customPlot->xAxis->setVisible(true);
    customPlot->yAxis->setVisible(true);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->replot();
}

void Hybrid_UI::Set_UI_Reactant_Table(Ui::Hybrid * ui, int Reactant_number, bool From_excel, QList<QList<QVariant>> excel_model)
{
    ui->tableWidget_R_N->setRowCount(Reactant_number);
    ui->tableWidget_R_N->setColumnCount(2);
    ui->tableWidget_R_N->resizeColumnsToContents();
    ui->tableWidget_R_N->resizeRowsToContents();

    for (int i = 0; i < Reactant_number; i++) // Set defaut values
    {
        if (From_excel==1)
        {
            ui->tableWidget_R_N->setItem(i,0,new QTableWidgetItem(excel_model.at(0).at(i+1).toString()));
        }
        else
        {
            ui->tableWidget_R_N->setItem(i,0,new QTableWidgetItem(""));
        }
        ui->tableWidget_R_N->setItem(i,1,new QTableWidgetItem("0"));
    }
}

void Hybrid_UI::Set_UI_Reaction_Table(Ui::Hybrid * ui, int Reactant_number, int Reaction_channles, bool From_excel, QList<QList<QVariant>> excel_model) // Note it is Ui::Hybrid * ui, but not Hybrid * ui !!!
{
    // Set reaction table
    ui->tableWidget_R_S_C->setRowCount(Reaction_channles);
    ui->tableWidget_R_S_C->setColumnCount(Reactant_number);
    ui->tableWidget_P_S_C->setRowCount(Reaction_channles);
    ui->tableWidget_P_S_C->setColumnCount(Reactant_number);
    ui->tableWidget_R_S_C->resizeColumnsToContents();
    ui->tableWidget_R_S_C->resizeRowsToContents();
    ui->tableWidget_P_S_C->resizeColumnsToContents();
    ui->tableWidget_P_S_C->resizeRowsToContents();

    // Set rate table
    ui->tableWidget_R_rate->setRowCount(Reaction_channles);
    ui->tableWidget_R_rate->setColumnCount(1);
    ui->tableWidget_R_rate->resizeColumnsToContents();
    ui->tableWidget_R_rate->resizeRowsToContents();
    QStringList vHeader_rate; vHeader_rate << "Rate";
    ui->tableWidget_R_rate->setHorizontalHeaderLabels(vHeader_rate);

    // Add reaction name into table
    QStringList vHeader;
    for (int i = 0; i < Reactant_number; i++) // Get name from reactant table
    {
        if (ui->tableWidget_R_N->item(i,0)->text()==QString(""))
        {
            QMessageBox::warning(NULL,"Warning","Please write reaction name first!");
        }
        else
        {
            vHeader << ui->tableWidget_R_N->item(i,0)->text();
        }
    }
    ui->tableWidget_R_S_C->setHorizontalHeaderLabels(vHeader);
    ui->tableWidget_P_S_C->setHorizontalHeaderLabels(vHeader);

    // Add defaut reaction number into table
    for (int i = 0; i < Reaction_channles; i++)
    {
        for (int j = 0; j < Reactant_number; j++)
        {
            ui->tableWidget_R_S_C->setItem(i,j,new QTableWidgetItem("0"));
            ui->tableWidget_P_S_C->setItem(i,j,new QTableWidgetItem("0"));
        }
    }

    // Add defaut reaction rate into table
    for (int i = 0; i < Reaction_channles; i++)
    {
        ui->tableWidget_R_rate->setItem(i,0,new QTableWidgetItem("0"));
    }

    // Set plot comboBox
    ui->comboBox_Plot->clear();
    ui->comboBox_Plot->addItems(vHeader);

    // Import model form excel
    if (From_excel==1)
    {
        // Add defaut reaction number into table
        for (int i = 0; i < Reaction_channles; i++)
        {
            for (int j = 0; j < Reactant_number; j++)
            {
                ui->tableWidget_R_S_C->setItem(i,j,new QTableWidgetItem(excel_model.at(i+1).at(j+1).toString()));
                ui->tableWidget_P_S_C->setItem(i,j,new QTableWidgetItem(excel_model.at(i+1).at(j+1+Reactant_number).toString()));
            }
        }

        // Add defaut reaction rate into table
        for (int i = 0; i < Reaction_channles; i++)
        {
            ui->tableWidget_R_rate->setItem(i,0,new QTableWidgetItem(excel_model.at(i+1).at(0).toString()));
        }
    }
}

void Hybrid_UI::Plot(int K, QCustomPlot * customPlot, int count, double * t, double ** array)
{
    QVector<double> t_array, x_array;
    t_array.clear();
    x_array.clear();
    for (int i = 0; i < count; i++)
    {
        t_array.push_back(t[i]);
        x_array.push_back(array[K][i]);
    }

    customPlot->clearGraphs();
    customPlot->addGraph(); // Create graph and assign data to it;
    customPlot->graph(0)->setData(t_array, x_array);
    customPlot->rescaleAxes(); // NOTE!! MUST have this!!

    customPlot->xAxis->setLabel("time");
    customPlot->yAxis->setLabel("molecular number");
    customPlot->replot();
    customPlot->clearGraphs();
}

void Hybrid_UI::ExportData(int Reactant_number, Ui::Hybrid * ui, parameters_CRS * para_hyb, double * t, double ** array)
{
    std::ofstream dataFile[Reactant_number], dataFile_time;

    QString Name, F_name, F_name_0, F_name_1, F_name_2;
    F_name_1 = QFileDialog::getExistingDirectory(NULL,"Please select the path to save the results...","./");
    F_name_0 = F_name_1 + "/time.txt";
    F_name_2 = ".txt";

    // Export reactant number
    for (int i = 0; i < Reactant_number; i++)
    {
        // Open files
        Name = ui->tableWidget_R_N->item(i,0)->text();
        F_name = F_name_1 + "/" + Name + F_name_2;
        dataFile[i].open(F_name.toStdString());

        // Write data
        for (int j = 0; j < (*para_hyb).Ncyc; j++)
        {
            dataFile[i] << array[i][j] << std::endl;
        }

        // Close files
        dataFile[i].close();
    }

    // Export time
    dataFile_time.open(F_name_0.toStdString());
    for (int j = 0; j < (*para_hyb).Ncyc; j++)
    {
        dataFile_time << t[j] << std::endl;
    }
    dataFile_time.close();
}
