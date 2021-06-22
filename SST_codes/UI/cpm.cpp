#include "cpm.h"
#include "ui_cpm.h"

CPM::CPM(QWidget *parent) : QDialog(parent), ui(new Ui::CPM)
{
    ui->setupUi(this);
}

CPM::~CPM()
{
    delete ui;
}

void CPM::on_pushButton_Set_Background_clicked()
{
    // Get parameters
    QString str1 = ui->spinBox_1->text(); // 取出框架中的目前值
    x_pix = str1.toInt();

    QString str2 = ui->spinBox_2->text(); // 取出框架中的目前值
    y_pix = str2.toInt();

    img = new QImage(x_pix, y_pix, QImage::Format_ARGB32);
    img->fill(QColor(255, 255, 255, 255));

    QPixmap pixmap = QPixmap::fromImage(*img);
    ui->label->resize(5*img->width(),5*img->height()); // Important !!!
    ui->label->setPixmap(pixmap);
}

void CPM::on_pushButton_4_clicked()
{
    // Get parameters
    QString str1 = ui->spinBox_Single_Cell_Locx->text(); // 取出框架中的目前值
    x_loc = str1.toInt();

    QString str2 = ui->spinBox_Single_Cell_Locy->text(); // 取出框架中的目前值
    y_loc = str2.toInt();

    QString str3 = ui->spinBox_Single_Cell_R->text(); // 取出框架中的目前值
    R = str3.toInt();

    QString str4 = ui->spinBox_Cell_N->text(); // 取出框架中的目前值
    N = str4.toInt();

    // Initialize Background
    array.clear();array.squeeze();
    array = Initialization_Cell::Initialization_Cell_Circular(x_pix, y_pix, x_loc, y_loc, R, N);

    // Initialize parameter table
    ui->tableWidget->setRowCount(N+1); // 设置行数 注意包含基质！！
    ui->tableWidget->setColumnCount(N+1); // 设置列数
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();

    for (int i = 0; i < N+1; i++)
    {
        for (int j = 0; j < N+1; j++)
        {
            ui->tableWidget->setItem(i,j,new QTableWidgetItem("0"));
        }
    }

    QVector<QVector <double>> JJ(N+1);
    for (int i = 0; i < N+1; i++)
    {
        JJ[i].resize(N+1);
    }
    for (int i = 0; i < N+1; i++)
    {
        for (int j = 0; j < N+1; j++)
        {
            JJ[i][j] = 0.0;
        }
    }
    J = JJ;

    // Initialize parameter table
    ui->tableWidget_2->setRowCount(N);
    ui->tableWidget_2->resizeColumnsToContents();
    ui->tableWidget_2->resizeRowsToContents();

    for (int i = 0; i < N; i++)
    {
        ui->tableWidget_2->setItem(i,0,new QTableWidgetItem("50"));
        ui->tableWidget_2->setItem(i,1,new QTableWidgetItem("1"));
    }

    QVector <double> lamda_VV(N);
    QVector <double> lamda_BB(N);
    for (int i = 0; i < N; i++)
    {
            lamda_VV[i] = 50.0;
            lamda_BB[i] = 1.0;
    }
    lamda_V = lamda_VV;
    lamda_B = lamda_BB;

    QVector <double> lamda_Chem_FF(2);
    lamda_Chem_FF.push_back(0.0);
    lamda_Chem_FF.push_back(1.0);
    lamda_Chem_Force = lamda_Chem_FF;

    // Set Color
    QVector <QColor> tqc(N);
    tqc.clear();
    for (int i = 0; i < N; i++)
    {
        tqc.push_back(QColor(rand()%256,rand()%256,rand()%256));
    }
    qc = tqc;

    // Initialize Boader
    Boader = Find_neighborhood::Find_Boader_Von_Neumann(x_pix, y_pix, array);// h换Von_Neumann为啥不行？？

    // 实体绘图
    Plot_CPM::Plot_Cells(x_pix, y_pix, N, &array, img, &qc);

    // 边界绘图
    Plot_CPM::Plot_Boader(x_pix, y_pix, N, &array, &Boader, img, &qc);

    QPixmap pixmap = QPixmap::fromImage(*img);
    ui->label->setPixmap(pixmap);

    // Calculate target area (2D) / volume (3D)
    QVector <int> Volume(N);
    Volume.clear();
    for (int j = 1; j <= N; j++)
    {
        Volume.push_back(0);
    }

    for (int i = 0; i < x_pix * y_pix; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            if (array[i][3] == j)
            {
                // Notice is [j-1]
                Volume[j-1] = Volume[j-1] + 1;
            }
        }
    }

    Target_Volume = Volume;

    // Calculate target perimeter (2D) / surface area (3D)
    QVector <int> Perimeter(N);
    Perimeter.clear();
    for (int j = 1; j <= N; j++)
    {
        Perimeter.push_back(0);
    }

    for (int i = 0; i < Boader.size(); i++)
    {
        int aa = Boader[i][0]; //要能全覆盖
        int bb = Boader[i][1];

        for (int j = 1; j <= N; j++)
        {
            if (array[aa][3] == j || array[bb][3] == j)
            {
                // Notice is [j-1]
                Perimeter[j-1] = Perimeter[j-1] + 1;
            }
        }
    }

    Target_S = Perimeter;
}

void CPM::on_pushButton_clicked()
{
    // Main
    count = 0;
    timer = new QTimer(this);
    timer->start(10);
    connect(timer,SIGNAL(timeout()),this,SLOT(Main_Cycle()));
}

void CPM::Main_Cycle()
{
    if (count > Sim_time) timer->stop();
    count = count + 1;

    // 边界移动
    double r1 = Basic_Class::random_0_to_1(); // (0,1) 不包括0,1
    int rand_chose = ceil(r1 * Boader.size())-1; // 超级大bug，这里应该-1！！！！
    //qDebug()<<rand_chose<<"|"<<Boader.size()<<endl;
    aa = Boader[rand_chose][0]; //要能全覆盖 target_pixel
    bb = Boader[rand_chose][1]; // move_pixel
    //qDebug()<<aa<<"|"<<bb;

    double r2 = Basic_Class::random_0_to_1();
    int target_pixel = 0;
    int move_pixel = 0;
    if (r2 < 0.5)
    {
        target_pixel = aa;
        move_pixel = bb;
    }
    else
    {
        target_pixel = bb;
        move_pixel = aa;
    }

    // 记录尝试变化
    temple_id = array[target_pixel][3];

    // 记录粘附能变化
    derta_Adh = Hamiltonian::Derta_Adhesion(x_pix, y_pix, target_pixel, move_pixel, &array, &J); // 注意，里面修改了array，必须传指针！！！

    // 计算趋化性
    derta_Chem_Force = Hamiltonian::Derta_Chemcial_Force(&lamda_Chem_Force, target_pixel, move_pixel, &array);

    // 更新边界，以便进行计算 (这个计算量大，后面想想怎么替换下算法，只更新新边界就行)
    Boader.clear();Boader.squeeze();
    Boader = Find_neighborhood::Find_Boader_Von_Neumann(x_pix, y_pix, array);

    // 计算体积能
    derta_V = Hamiltonian::Volume_Constraint(x_pix, y_pix, N, &lamda_V, &array, &Target_Volume);

    // 计算周长能
    derta_B = Hamiltonian::Perimeter_Constraint(x_pix, y_pix, N, &lamda_B, &array, &Target_S, &Boader);

    // 计算总能量差
    derta_H = derta_Adh + derta_V + derta_B + derta_Chem_Force;

    // 计算改变概率
    derta_P = exp(-derta_H/T);

    //qDebug()<<derta_H<<"|"<<derta_V<<"|"<<derta_B<<"|"<<derta_Adh<<"|"<<derta_Chem_Force;

    double r3 = Basic_Class::random_0_to_1();
    // derta_P 越小，稳定，恢复
    // 恢复算法
    if (derta_H > 0 && r3 > derta_P)
    {
        //Boader.push_back({tem_aa, tem_bb});

        //for (int i = 0; i < Boader_add_num; i++)
        //{
        //    Boader.removeLast();
        //} // IMPORTANT!!!

        if (r2 < 0.5)
        {
            array[aa][3] = temple_id;
        }
        else
        {
            array[bb][3] = temple_id;
        }

        // 重新确定边界
        Boader.clear();
        Boader.squeeze();
        Boader = Find_neighborhood::Find_Boader_Von_Neumann(x_pix, y_pix, array);
    }

    // 实体绘图
    Plot_CPM::Plot_Cells(x_pix, y_pix, N, &array, img, &qc);

    // 边界绘图
    Plot_CPM::Plot_Boader(x_pix, y_pix, N, &array, &Boader, img, &qc);

    QPixmap pixmap = QPixmap::fromImage(*img);
    ui->label->setPixmap(pixmap);

    update();
}

void CPM::on_pushButton_2_clicked()
{
    QString str6 = ui->spinBox_time->text(); // 取出框架中的目前值
    Sim_time = str6.toInt();

    QString str8 = ui->doubleSpinBox_T->text(); // 取出框架中的目前值
    T = str8.toDouble();

    for (int i = 0; i < N+1; i++) // 注意是N+1！！别弄错了！！！
    {
        for (int j = 0; j < N+1; j++)
        {
            QString str = ui->tableWidget->item(i,j)->text(); // 取出分子数字符串
            double num = str.toDouble(); // 转成int
            J[i][j] = num;
        }
    }

    for (int i = 0; i < N; i++)
    {
        QString str12 = ui->tableWidget_2->item(i,0)->text(); // 取出框架中的目前值
        lamda_V[i] = str12.toDouble();
        QString str13 = ui->tableWidget_2->item(i,1)->text(); // 取出框架中的目前值
        lamda_B[i] = str13.toDouble();
    }

    QString str14 = ui->doubleSpinBox_6->text(); // 取出框架中的目前值
    lamda_Chem_Force[0] = str14.toDouble(); // 化学趋性
    QString str15 = ui->doubleSpinBox_7->text(); // 取出框架中的目前值
    lamda_Chem_Force[1] = str15.toDouble(); // 物理趋性
}
