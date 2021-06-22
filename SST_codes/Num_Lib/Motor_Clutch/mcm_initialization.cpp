#include "Num_Lib/Motor_Clutch/mcm_initialization.h"

MCM_Initialization::MCM_Initialization()
{

}

iteration MCM_Initialization::Iteration(parameters * para)
{
    // Iteration variable MUST RESET in here !!!!
    iteration iter =
    {
        .t_and_tmin = {0.0, 0.0},
        .ECMloc = 0.0,
        .Traction_old = 0.0,
        .Traction = 0.0,
        .Vactin = 0.0,
        .F_K_sum = {0.0, 0.0},
        .Iteration_variable = {0.0}, // 其余8个默认为0
        .dint = new double [(*para).Type_number],
        .kon = new double [(*para).Type_number],
        .Nmyosin_eff = (*para).Nmyosin,
        .Averageforce = new double [(*para).Type_number],
        .TF = new double [(*para).Type_number],
        .N_link = new double [(*para).Type_number],
        .Ncycle = 0.0,
        .Active_sites = 0
    };

    // Dynamic array assignment，MUST！！
    for (int i = 0; i < (*para).Type_number; i++)
    {
        iter.dint[i] = (*para).dint[i];
        iter.kon[i] = 0.0;
        iter.Averageforce[i] = 0.0;
        iter.TF[i] = 0.0;
        iter.N_link[i] = 0.0;
    }

    return iter;
}

void MCM_Initialization::Parameters(parameters * para, Ui::MCM * ui, int type_number, int num_links, int simulation_cycle)
{
    // Set base parameters
    (*para).Type_number = type_number;
    (*para).Num_links = num_links;
    (*para).Ncyc = simulation_cycle;
    (*para).Out_result = 0;

    // Set bond parameters
    (*para).Klink = new double [(*para).Type_number];
    (*para).Bond_type = new int [(*para).Type_number];
    (*para).Fcr = new double [(*para).Type_number];
    (*para).dint = new double [(*para).Type_number];
    (*para).dadd = new double [(*para).Type_number];
    (*para).dmin = new double [(*para).Type_number];
    (*para).dmax = new double [(*para).Type_number];
    (*para).kont = new double [(*para).Type_number];
    (*para).koff0 = new double [(*para).Type_number];
    (*para).kF = new double [(*para).Type_number];
    (*para).koff1 = new double [(*para).Type_number];
    (*para).koff2 = new double [(*para).Type_number];
    (*para).kF1 = new double [(*para).Type_number];
    (*para).kF2 = new double [(*para).Type_number];

    for (int i = 0; i < (*para).Type_number; i++)
    {
        int k = i+1; QString str1 = ""; double num1 = 0.0; int num2 = 0;
        str1 = ui->tableWidget_1->item(0,k)->text(); num1 = str1.toDouble(); (*para).Klink[i] = num1;
        str1 = ui->tableWidget_1->item(1,k)->text(); num1 = str1.toDouble(); (*para).dint[i] = num1;
        str1 = ui->tableWidget_1->item(2,k)->text(); num1 = str1.toDouble(); (*para).kont[i] = num1;
        str1 = ui->tableWidget_1->item(3,k)->text(); num2 = str1.toInt(); (*para).Bond_type[i] = num2;
        str1 = ui->tableWidget_2->item(0,k)->text(); num1 = str1.toDouble(); (*para).koff0[i] = num1;
        str1 = ui->tableWidget_2->item(1,k)->text(); num1 = str1.toDouble(); (*para).kF[i] = num1;
        str1 = ui->tableWidget_3->item(0,k)->text(); num1 = str1.toDouble(); (*para).koff1[i] = num1;
        str1 = ui->tableWidget_3->item(1,k)->text(); num1 = str1.toDouble(); (*para).koff2[i] = num1;
        str1 = ui->tableWidget_3->item(2,k)->text(); num1 = str1.toDouble(); (*para).kF1[i] = num1;
        str1 = ui->tableWidget_3->item(3,k)->text(); num1 = str1.toDouble(); (*para).kF2[i] = num1;
        str1 = ui->tableWidget_4->item(0,k)->text(); num1 = str1.toDouble(); (*para).Fcr[i] = num1;
        str1 = ui->tableWidget_4->item(1,k)->text(); num1 = str1.toDouble(); (*para).dadd[i] = num1;
        str1 = ui->tableWidget_4->item(2,k)->text(); num1 = str1.toDouble(); (*para).dmin[i] = num1;
        str1 = ui->tableWidget_4->item(3,k)->text(); num1 = str1.toDouble(); (*para).dmax[i] = num1;
    }

    // Set ECM mechanics and other para
    (*para).ECM_type = ui->comboBox_4->currentIndex() + 1;
    QString str1; double num1;
    str1 = ui->tableWidget_5->item(0,1)->text(); num1 = str1.toDouble(); (*para).E1 = num1;
    str1 = ui->tableWidget_5->item(0,2)->text(); num1 = str1.toDouble(); (*para).E2 = num1;
    str1 = ui->tableWidget_5->item(0,3)->text(); num1 = str1.toDouble(); (*para).R1 = num1;
    str1 = ui->tableWidget_5->item(0,4)->text(); num1 = str1.toDouble(); (*para).R2 = num1;
    str1 = ui->tableWidget_6->item(0,1)->text(); num1 = str1.toDouble(); (*para).V0 = num1;
    str1 = ui->tableWidget_6->item(1,1)->text(); num1 = str1.toDouble(); (*para).Nmyosin = num1;
    str1 = ui->tableWidget_6->item(2,1)->text(); num1 = str1.toDouble(); (*para).Fstall = num1;
    str1 = ui->tableWidget_7->item(0,1)->text(); num1 = str1.toDouble(); (*para).Myosin_loop = num1;
    str1 = ui->tableWidget_7->item(1,1)->text(); num1 = str1.toDouble(); (*para).Myo_dadd = num1;
    str1 = ui->tableWidget_7->item(2,1)->text(); num1 = str1.toDouble(); (*para).Myo_min = num1;
    str1 = ui->tableWidget_7->item(3,1)->text(); num1 = str1.toDouble(); (*para).Myo_max = num1;
}
