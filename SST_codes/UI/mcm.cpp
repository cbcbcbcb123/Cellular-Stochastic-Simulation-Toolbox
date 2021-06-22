#include "UI/mcm.h"
#include "ui_mcm.h"

MCM::MCM(QWidget *parent) : QWidget(parent), ui(new Ui::MCM)
{
    ui->setupUi(this);

    // Set mainwindow for MCM-UI
    customPlot = new QCustomPlot(this);
    MC_results = new QTextBrowser(this);
    MCM_UI::Set_UI_Main(ui, customPlot, MC_results);
}

MCM::~MCM()
{
    delete ui;
}

//********************************************//
// Initialize parameters
//********************************************//
void MCM::on_pushButton_para_clicked()
{
    // Get base parameter
    QString str1 = ui->spinBox_type_number->text();
    type_number = str1.toInt();
    QString str2 = ui->spinBox_num_links->text();
    num_links = str2.toInt();
    QString str3 = ui->spinBox_simulation_cycle->text();
    simulation_cycle = str3.toInt();

    MCM_UI::Set_UI_Parameters(ui, type_number);

    QMessageBox::information(NULL, "Check Parameters", "Done !");
}

//********************************************//
// Run simulator
//********************************************//
void MCM::on_pushButton_run_clicked()
{
    // 0.Initialization
    // 0.1 Get simulation parameters from UI
    MCM_Initialization::Parameters(&para, ui, type_number, num_links, simulation_cycle);

    // 0.2 Initialization result pointer, used to export results
    // 0.21 Output time-dependent variables
    double (*Results)[8] = new double [para.Ncyc][8];
    for (int i = 0; i < para.Ncyc; i++)  // Initiation!!! VERY IMPORTANT!!!
    {
        for (int j = 0; j < 8; j++)
        {
            Results[i][j] = 0.0;
        }
    }
    // 0.22 Output average values of variables
    // [1.force, 2.flow, 3.adhesion, 4.effective spring, 5.cycle time, 6.active sties]
    double AV_results[6] = {0.0,0.0,0.0,0.0,0.0,0.0};

    // 1.Run simulation
    MainLoop::MC(&para, Results, AV_results);

    // 2.Plot
    // MUST clear array first !!!
    x1.clear(); x2.clear(); x3.clear(); x4.clear(); x5.clear(); x6.clear(); x7.clear(); x8.clear();
    for (int i = 0; i < para.Ncyc; i++)
    {
        x1.push_back(Results[i][0]);
        x2.push_back(Results[i][1]);
        x3.push_back(Results[i][2]);
        x4.push_back(Results[i][3]);
        x5.push_back(Results[i][4]);
        x6.push_back(Results[i][5]);
        x7.push_back(Results[i][6]);
        x8.push_back(Results[i][7]*Results[i][3]);
    }

    // 3.Output into textbrower
    MC_results->append("--BEGINE--");
    MC_results->append("Averate traction is ");
    MC_results->append(QString::number (AV_results[0],'g',6)); // double to Qstring
    MC_results->append("Averate actin flow is ");
    MC_results->append(QString::number (AV_results[1],'g',6)); // double to Qstring
    MC_results->append("Averate adhesion area/length is ");
    MC_results->append(QString::number (AV_results[2],'g',6)); // double to Qstring
    MC_results->append("Averate active receptor in adhesion is ");
    MC_results->append(QString::number (AV_results[5]*AV_results[2],'g',6)); // double to Qstring
    MC_results->append("--END--");

    // 4.Clear
    // 结构体中成员变量中存在指针的，其内存释放遵循从里向外的原则，即先释放成员变量的内存，然后在释放结构体指针。
    // 由于C++中允许结构体中使用构造函数和析构函数，构造函数对成员数据进行初始化，析构函数对内存进行清理。
    // 析构函数在对结构体进行释放时调用，首先释放析构函数中的成员变量内存，然后释放结构体指针内存。
    // 注意，本例中不能进行delete &para !!!
    delete [] para.Klink;
    delete [] para.Bond_type;
    delete [] para.Fcr;
    delete [] para.dint;
    delete [] para.dadd;
    delete [] para.dmin;
    delete [] para.dmax;
    delete [] para.kont;
    delete [] para.koff0;
    delete [] para.kF;
    delete [] para.koff1;
    delete [] para.koff2;
    delete [] para.kF1;
    delete [] para.kF2;

    // 5.Give message
    QMessageBox::information(NULL, "Run simulation", "Done !");
}

//********************************************//
// Plot time-dependent vars
//********************************************//
void MCM::on_pushButton_plot_1_clicked()
{
    int K = ui->comboBox->currentIndex();
    MCM_UI::Plot_1(K, customPlot, x1, x2, x3, x4, x8);
}

//********************************************//
// Save time-dependent vars
//********************************************//
void MCM::on_pushButton_save_1_clicked()
{
    MCM_UI::Save_Data_1(&para, x1, x2, x3, x4, x8);
}

//********************************************//
// Sensitivity with ECM mechanics
//********************************************//
void MCM::on_pushButton_analysis_clicked()
{
    // 0.Initialization
    // 0.1 Get simulation parameters from UI
    MCM_Initialization::Parameters(&para, ui, type_number, num_links, simulation_cycle);

    Para_mechanosen_ID = ui->comboBox_5->currentIndex();
    value_down = ui->doubleSpinBox_1->text().toDouble();
    value_up = ui->doubleSpinBox_2->text().toDouble();
    dvalue = ui->doubleSpinBox_3->text().toDouble();

    // MUST clear array first !!!
    // Very Very Important, 缺失了这部将导致数据不断网上加！！！
    a0.clear(); a1.clear(); a2.clear(); a3.clear(); a4.clear(); a5.clear(); a6.clear();

    double dv = (value_up - value_down)/dvalue;
    double Progress_N = 0;

    for (double i = value_down; i <= value_up; i = i + dv)
    {
        // 0.2 Select parameters for sensitivity
        if (Para_mechanosen_ID == 0)
        {
            para.E1 = pow(10,i);
        }
        if (Para_mechanosen_ID == 1)
        {
            para.E2 = pow(10,i);
        }
        if (Para_mechanosen_ID == 2)
        {
            para.R1 = pow(10,i);
        }
        if (Para_mechanosen_ID == 3)
        {
            para.R2 = pow(10,i);
        }

        // 1.Initialization result pointer, used to export results
        // 1.1 Output variables with time
        double (*Results)[8] = new double [para.Ncyc][8];
        for (int i = 0; i < para.Ncyc; i++)  // Initiation!!! VERY IMPORTANT
        {
            for (int j = 0; j < 8; j++)
            {
                Results[i][j] = 0;
            }
        }
        // 1.2 Output average values of variables
        // [1.force, 2.flow, 3.adhesion, 4.effective spring, 5.cycle time, 6.active sties]
        double AV_results[6] = {0.0,0.0,0.0,0.0,0.0,0.0};

        // 2.Run simulation
        MainLoop::MC(&para, Results, AV_results);

        // 3.Plot
        a0.push_back(i);
        a1.push_back(AV_results[0]);
        a2.push_back(AV_results[1]);
        a3.push_back(AV_results[2]);
        a4.push_back(AV_results[3]);
        a5.push_back(AV_results[4]);
        a6.push_back(AV_results[5]*AV_results[2]);

        // 4.Set progress bar
        Progress_N = Progress_N + 1;
        double dProgress = (Progress_N/dvalue) * 100;
        ui->progressBar->setValue(dProgress);
    }

    // 5.Clear
    delete [] para.Klink;
    delete [] para.Bond_type;
    delete [] para.Fcr;
    delete [] para.dint;
    delete [] para.dadd;
    delete [] para.dmin;
    delete [] para.dmax;
    delete [] para.kont;
    delete [] para.koff0;
    delete [] para.kF;
    delete [] para.koff1;
    delete [] para.koff2;
    delete [] para.kF1;
    delete [] para.kF2;
}

//********************************************//
// Plot average vars
//********************************************//
void MCM::on_pushButton_plot_2_clicked()
{
    int K = ui->comboBox_2->currentIndex();
    MCM_UI::Plot_2(K, customPlot, a0, a1, a2, a3, a6);
}

//********************************************//
// Save average vars
//********************************************//
void MCM::on_pushButton_save_2_clicked()
{
    MCM_UI::Save_Data_2(&a1, &a2, &a3, &a4, &a5, &a6, value_up, value_down, dvalue);
}
