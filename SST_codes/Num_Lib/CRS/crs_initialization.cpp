#include "Num_Lib/CRS/crs_initialization.h"

CRS_Initialization::CRS_Initialization()
{

}

void CRS_Initialization::Parameters(iteration_CRS * iter_hyb, parameters_CRS * para_hyb)
{
    // Define iteration varibles
    (*iter_hyb) =
    {
        .Num_molecules = {},
        .Num_molecules_low = {},
        .Num_molecules_high = {},
        .t = 0.0,
        .tmin = 0.0,
        .X_min_loc = 0,
        .cal_stop = false,
        .RSSA_update_number = true
    };

    // Define parameter varibles
    (*para_hyb) =
    {
        .N_r = 0,
        .M_r = 0,
        .Ncyc = 0,
        .Methods = "ODE",
        .t = 0.0,
        .h = 0.1,
        .eps = 0.00001
    };
}

bool CRS_Initialization::Reactions(QVector<struct reaction> * reaction_collection_hyb, iteration_CRS * iter_hyb, parameters_CRS * para_hyb, Ui::Hybrid * ui, int Reactant_number, int Reaction_channles, bool Check_3)
{
    // Initialization of reactant number
    for (int i = 0; i < Reactant_number; i++)
    {
        QString str = ui->tableWidget_R_N->item(i,1)->text();
        int num = str.toInt();
        (*iter_hyb).Num_molecules.push_back(num);
        (*iter_hyb).Num_molecules_low.push_back(num);
        (*iter_hyb).Num_molecules_high.push_back(num);
    }

    // Initialization of simulation parameters
    QString str1 = ui->spinBox_Cycle->text();
    (*para_hyb).Ncyc = str1.toInt();
    QString str2 = ui->doubleSpinBox_Ini_Time->text();
    (*para_hyb).t = str2.toDouble();
    QString str3 = ui->doubleSpinBox_ODE_Step->text();
    (*para_hyb).h = str3.toDouble();
    QString str4 = ui->doubleSpinBox_Error->text();
    (*para_hyb).eps = str4.toDouble();

    (*para_hyb).N_r = Reactant_number;
    (*para_hyb).M_r = Reaction_channles;

    // Note: Structure initialization is very important !
    for (int i = 0; i < Reaction_channles; i++)
    {
        (*reaction_collection_hyb).push_back({{}, 0.0, {}, 0.0, 0.0, 0, 0.0, 0.0, 0.0, 0.0});
    }

    // Initialization of rates
    for (int i = 0; i < Reaction_channles; i++)
    {
        QString str_r = ui->tableWidget_R_rate->item(i,0)->text();
        double num_r = str_r.toDouble();
        (*reaction_collection_hyb)[i].rate = num_r;
    }

    // Initialization of stoichiometry of reactant
    for (int i = 0; i < Reaction_channles; i++)
    {
        for (int j = 0; j < Reactant_number; j++)
        {
            QString str = ui->tableWidget_R_S_C->item(i,j)->text();
            int num = str.toInt();

            if (num <= 3)
            {
                (*reaction_collection_hyb)[i].SS.push_back(num);
            }
            else
            {
                QMessageBox::warning(NULL,"Warning","Reaction coefficient should be not greater than 3! Please set again");
                Check_3 = false;
            }
        }
    }

    // Initialization of state change vector
    for (int i = 0; i < Reaction_channles; i++)
    {
        for (int j = 0; j < Reactant_number; j++)
        {
            QString str1 = ui->tableWidget_R_S_C->item(i,j)->text();
            QString str2 = ui->tableWidget_P_S_C->item(i,j)->text();
            int num1 = str1.toInt();
            int num2 = str2.toInt();
            int dnum = num2 - num1;
            (*reaction_collection_hyb)[i].vmu.push_back(dnum);
        }
    }

    // Initialization of reaction types (stochasitc or derterministic)
    for (int i = 0; i < Reaction_channles; i++)
    {
        (*reaction_collection_hyb)[i].D_S = 0;
    }

    return Check_3;
}

void CRS_Initialization::Methods_and_classifier(QVector<struct reaction> * reaction_collection_hyb, iteration_CRS * iter_hyb, parameters_CRS * para_hyb, Ui::Hybrid * ui, int Reaction_channles)
{
    // Set reaction methods
    QString XX =  ui->comboBox->currentText();
    (*para_hyb).Methods = XX.toStdString();

    // Set classifier for reactions
    if ((*para_hyb).Methods == "ODE" || (*para_hyb).Methods == "tODE" || (*para_hyb).Methods == "DM-ODE")
    {
        for (int i = 0; i < Reaction_channles; i++)
        {
            (*reaction_collection_hyb)[i].D_S = 0;
        }
    }

    if ((*para_hyb).Methods == "DM" || (*para_hyb).Methods == "RSSA" || (*para_hyb).Methods == "tRSSA")
    {
        for (int i = 0; i < Reaction_channles; i++)
        {
            (*reaction_collection_hyb)[i].D_S = 1;
        }
    }
}
