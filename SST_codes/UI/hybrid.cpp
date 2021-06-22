#include "hybrid.h"
#include "ui_hybrid.h"

Hybrid::Hybrid(QWidget * parent) : QDialog(parent), ui(new Ui::Hybrid)
{
    ui->setupUi(this);

    // Set subwindows in main interface
    customPlot = new QCustomPlot(this);
    Hybrid_UI::Set_UI_Main(ui, customPlot);
}

Hybrid::~Hybrid()
{
    delete ui;
}

void Hybrid::on_pushButton_Ini_tables_clicked()
{
    // Initialize chemical reaction pointer
    reaction_collection_hyb = new QVector<struct reaction>;
    // Initialize "para" and "inter" variables
    CRS_Initialization::Parameters(&iter_hyb, &para_hyb);

    // Check
    Check_1 = true;

    // Get reactant name and number
    QString str1 = ui->spinBox_R_N->text();
    Reactant_number = str1.toInt();
    QString str2 = ui->spinBox_R_C->text();
    Reaction_channles = str2.toInt();

    // Set reactant name | number table
    Hybrid_UI::Set_UI_Reactant_Table(ui, Reactant_number, From_excel, excel_model);
}

void Hybrid::on_pushButton_Set_Reaction_clicked()
{
    if (Check_1 == true)
    {
        // Check
        Check_2 = true;

        Hybrid_UI::Set_UI_Reaction_Table(ui, Reactant_number, Reaction_channles, From_excel, excel_model);
    }
    else
    {
        QMessageBox::warning(NULL,"Warning","Please press <Initialization Button> first!");
    }
}

void Hybrid::on_pushButton_Check_clicked()
{
    if (Check_2 == true && Check_4 == true)
    {
        // Check done
        Check_3 = true;

        // Set reactions from UI
        Check_3 = CRS_Initialization::Reactions(reaction_collection_hyb, &iter_hyb, &para_hyb, ui, Reactant_number, Reaction_channles, Check_3);

        // Initialization of result record vector
        t = new double [para_hyb.Ncyc]; // DO NOT place this into CRS_Output::Initialization !!
        array = new double * [Reactant_number]; // DO NOT place this into CRS_Output::Initialization !!
        Check_4 = false;
        CRS_Output::Initialization(&para_hyb, Reactant_number, t, array);

        // Set simulation method and classifier of reactions
        CRS_Initialization::Methods_and_classifier(reaction_collection_hyb, &iter_hyb, &para_hyb, ui, Reaction_channles);

        // Import time-dependent rate from excel
        if (para_hyb.Methods == "tODE" || para_hyb.Methods == "tRSSA")
        {
            excel_list.clear();

            Hybrid::Import_Time_Dependent_Rate();

            QMessageBox::information(NULL, "Read rate parameter", "Done");
        }
    }
    else if (Check_2 == false)
    {
        QMessageBox::warning(NULL,"Warning","Please press <Input Chemical Reaction Button> first!");
    }
    else if (Check_4 == false)
    {
        QMessageBox::warning(NULL,"Warning","Please press <Clear Button> first!");
    }
}

void Hybrid::on_pushButton_Run_clicked()
{
    if (Check_3 == true)
    {
        if (para_hyb.Methods == "ODE") // time-independent ODE Method
        {
            ODE_Solver::Time_Independent_ODE(ui, reaction_collection_hyb, &iter_hyb, &para_hyb, Reactant_number, t, array);
        }
        else if (para_hyb.Methods == "tODE") // time-dependent ODE Method
        {
            ODE_Solver::Time_Dependent_ODE(ui, &excel_list, row_count, reaction_collection_hyb, &iter_hyb, &para_hyb, Reactant_number, t, array);
        }
        else if (para_hyb.Methods == "DM") // DM Method
        {
            SSA_Solver::DM(ui, reaction_collection_hyb, &iter_hyb, &para_hyb, Reactant_number, t, array);
        }
        else if (para_hyb.Methods == "DM-ODE") // ODE-DM Method
        {
            SSA_Solver::ODE_DM(ui, reaction_collection_hyb, &iter_hyb, &para_hyb, Reactant_number, t, array);
        }
        else if (para_hyb.Methods == "RSSA" || para_hyb.Methods == "tRSSA") // RSSA Method
        {
            SSA_Solver::RSSA(ui, reaction_collection_hyb, &iter_hyb, &para_hyb, Reactant_number, t, array, row_count, &excel_list);
        }

        // Clear pointer and data of last simulation
        // Delete reaction_collection; NO NOT use delete [] reaction_collection, since it is not a pointer, its a vector
        iter_hyb.Num_molecules.clear(); // MUST place here
        iter_hyb.Num_molecules_low.clear();
        iter_hyb.Num_molecules_high.clear();
        iter_hyb.t = 0.0;
        iter_hyb.cal_stop = false;
        (*reaction_collection_hyb).clear(); // MUST place here
        Check_3 = false;
    }
    else
    {
        QMessageBox::warning(this,"Warning","Please press <Clear Button> first!");
    }
}

void Hybrid::on_pushButton_Plot_clicked()
{
    if (Check_4 == false)
    {
        int K = ui->comboBox_Plot->currentIndex();
        Hybrid_UI::Plot(K, customPlot, para_hyb.Ncyc, t, array);
    }
    else
    {
        QMessageBox::warning(this,"Warning","Please press <Clear Button> first!");
    }
}

void Hybrid::on_pushButton_Output_clicked()
{
    if (Check_4 == false)
    {
        Hybrid_UI::ExportData(Reactant_number, ui, &para_hyb, t, array);
        QMessageBox::information(this, "Message", "Export data done!");
    }
    else
    {
        QMessageBox::warning(this,"Warning","Please press <Clear Button> first!");
    }
}

void Hybrid::on_pushButton_Clear_clicked()
{
    // Clear Pointer
    if (t != nullptr)
    {
        delete [] t;
        t = nullptr;
    }

    for(int i = 0; i < Reactant_number; i++)
    {
        if (array[i] != nullptr)
        {
            delete [] array[i];
            array[i] = nullptr;
        }
    }
    if (array != nullptr)
    {
        delete [] array;
        array = nullptr;
    }

    Check_4 = true;
}

void Hybrid::Import_Time_Dependent_Rate()
{
    QAxObject * excel = new QAxObject(); // 建立excel操作对象
    excel->setControl("Excel.Application"); // 连接Excel控件
    excel->setProperty("Visible", false); // 显示窗体看效果,选择true将会看到excel表格被打开
    excel->setProperty("DisplayAlerts", true); // 显示警告看效果
    QAxObject * workbooks = excel->querySubObject("WorkBooks"); // 获取工作簿(excel文件)集合
    QString str = QFileDialog::getOpenFileName(NULL,"打开",QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),"Excel 文件(*.xls *.xlsx)");
    workbooks->dynamicCall("Open(const QString&)", str); // 打开刚才选定的excel
    QAxObject * workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)",1);
    QAxObject * usedRange = worksheet->querySubObject("UsedRange"); // 获取表格中的数据范围

    // 记得添加 #include <QVariant> 否则会出错
    QVariant var = usedRange->dynamicCall("Value"); // 将所有的数据读取到QVariant容器中保存
    QVariantList varRows = var.toList();
    if(varRows.isEmpty())
    {
        return;
    }
    row_count = varRows.size();
    QVariantList rowData;
    for(int i = 0; i < row_count; ++i)
    {
        rowData = varRows[i].toList();
        excel_list.push_back(rowData);
    }

    // 将二维数组Qlist<Qlist<QVariant>>中的数据读取出来，到表格或者数据模型中，
    workbook->dynamicCall("Close(Boolean)", false);
    excel->dynamicCall("Quit(void)");
    delete excel; // 因为前面选择了不显示excel窗口，如果这里不删除excel的话，excel依然是打开的，只是我们看不见（因为被隐藏起来了）
}

void Hybrid::on_pushButton_SBML_File_clicked()
{
    excel_model.clear(); // Must add !!!

    QAxObject * excel = new QAxObject(); // 建立excel操作对象
    excel->setControl("Excel.Application"); // 连接Excel控件
    excel->setProperty("Visible", false); // 显示窗体看效果,选择true将会看到excel表格被打开
    excel->setProperty("DisplayAlerts", true); // 显示警告看效果
    QAxObject * workbooks = excel->querySubObject("WorkBooks"); // 获取工作簿(excel文件)集合
    QString str = QFileDialog::getOpenFileName(NULL,"打开",QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),"Excel 文件(*.xls *.xlsx)");
    workbooks->dynamicCall("Open(const QString&)", str); // 打开刚才选定的excel
    QAxObject * workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)",1);
    QAxObject * usedRange = worksheet->querySubObject("UsedRange"); // 获取表格中的数据范围

    // 记得添加 #include <QVariant> 否则会出错
    QVariant var = usedRange->dynamicCall("Value"); // 将所有的数据读取到QVariant容器中保存
    QVariantList varRows = var.toList();
    if(varRows.isEmpty())
    {
        return;
    }
    row_count_model = varRows.size(); // excel 行数 !!!!
    QVariantList rowData;
    for(int i = 0; i < row_count_model; ++i)
    {
        rowData = varRows[i].toList();
        column_count_model = rowData.size(); // excel 行数 !!!!
        excel_model.push_back(rowData);
    }

    // 将二维数组Qlist<Qlist<QVariant>>中的数据读取出来，到表格或者数据模型中，
    workbook->dynamicCall("Close(Boolean)", false);
    excel->dynamicCall("Quit(void)");
    delete excel; // 因为前面选择了不显示excel窗口，如果这里不删除excel的话，excel依然是打开的，只是我们看不见（因为被隐藏起来了）

    From_excel = true;
}
