#include "ssa_solver.h"
#include "UI/hybrid.h"

SSA_Solver::SSA_Solver()
{

}

void SSA_Solver::DM(Ui::Hybrid * ui, QVector<struct reaction> * reaction_collection_hyb, iteration_CRS * iter_hyb, parameters_CRS * para_hyb, int Reactant_number, double * t, double ** array)
{
    for (int i = 0; i < (*para_hyb).Ncyc; i++)
    {
        // Record results
        t[i] = (*iter_hyb).t;
        for (int j = 0; j < Reactant_number; j++)
        {
            array[j][i] = (*iter_hyb).Num_molecules[j];
        }

        // Simulation
        SSA_Solver::Update_amu(para_hyb, iter_hyb, reaction_collection_hyb);
        SSA_Solver::Reaction_Time_DM(para_hyb, iter_hyb, reaction_collection_hyb);

        // Set progress bar
        double dProgress = (double(i+1)/(*para_hyb).Ncyc) * 100;
        ui->progressBar->setValue(dProgress);
    }
}

void SSA_Solver::RSSA(Ui::Hybrid * ui, QVector<struct reaction> * reaction_collection_hyb, iteration_CRS * iter_hyb, parameters_CRS * para_hyb, int Reactant_number, double * t, double ** array, int row_count, QList<QList<QVariant>> * excel_list)
{
    for (int i = 0; i < (*para_hyb).Ncyc; i++)
    {
        // Record results
        t[i] = (*iter_hyb).t;
        for (int j = 0; j < Reactant_number; j++)
        {
            array[j][i] = (*iter_hyb).Num_molecules[j];
        }

        double deta = 0.1; // 10% based on current values

        if ((*iter_hyb).RSSA_update_number == true)
        {
            for (int j = 0; j < (*para_hyb).N_r; j++)
            {
                (*iter_hyb).Num_molecules_low[j] = (*iter_hyb).Num_molecules[j] * (1-deta);
                (*iter_hyb).Num_molecules_high[j] = (*iter_hyb).Num_molecules[j] * (1+deta);
            }

            (*iter_hyb).RSSA_update_number = false;
        }

        if ((*para_hyb).Methods == "RSSA")
        {
            SSA_Solver::Update_amu_RSSA(para_hyb, iter_hyb, reaction_collection_hyb);
        }
        else if ((*para_hyb).Methods == "tRSSA")
        {
            SSA_Solver::Update_amu_tRSSA(para_hyb, iter_hyb, reaction_collection_hyb, row_count, excel_list);
        }

        SSA_Solver::Reaction_Time_RSSA(para_hyb, iter_hyb, reaction_collection_hyb);

        // Set progress bar
        double dProgress = (double(i+1)/(*para_hyb).Ncyc) * 100;
        ui->progressBar->setValue(dProgress);
    }
}

void SSA_Solver::ODE_DM(Ui::Hybrid * ui, QVector<struct reaction> * reaction_collection_hyb, iteration_CRS * iter_hyb, parameters_CRS * para_hyb, int Reactant_number, double * t, double ** array)
{
    double y_pre[(*para_hyb).N_r];
    double a0_t = 0.0;
    for (int i = 0; i < (*para_hyb).Ncyc; i++)
    {
        // Record results
        t[i] = (*iter_hyb).t;
        for (int j = 0; j < Reactant_number; j++)
        {
            array[j][i] = (*iter_hyb).Num_molecules[j];
        }

        // 分类器
        if (i % 100 == 0)
        {
            SSA_Solver::Update_amu(para_hyb, iter_hyb, reaction_collection_hyb);

            for (int i = 0; i < (*para_hyb).M_r; i++)
            {
                if ((*reaction_collection_hyb)[i].amu * (*para_hyb).h < 5)
                {
                    (*reaction_collection_hyb)[i].D_S = 1;
                }
                else
                {
                    (*reaction_collection_hyb)[i].D_S = 0;
                }

                for (int j = 0; j < (*para_hyb).N_r; j++)
                {
                    if ((*iter_hyb).Num_molecules[j] < 100 * (*reaction_collection_hyb)[i].vmu[j]) // Note vmu not SS !!!!
                    {
                        (*reaction_collection_hyb)[i].D_S = 1;
                    }
                }
            }
        }

        if ((*iter_hyb).cal_stop == false)
        {
            // Set y_pre for returning to ODE calculation
            for (int k = 0; k < (*para_hyb).N_r; k++)
            {
                y_pre[k] = (*iter_hyb).Num_molecules[k];
            }

            // ODE step
            ODE_Solver::Treanor(para_hyb, iter_hyb, reaction_collection_hyb);
            (*iter_hyb).t = (*iter_hyb).t + (*para_hyb).h;

            // SSA step
            SSA_Solver::Update_amu(para_hyb, iter_hyb, reaction_collection_hyb);
            a0_t = SSA_Solver::Reaction_Time_DM_ODE(para_hyb, iter_hyb, reaction_collection_hyb, y_pre, a0_t);
        }

        // Set progress bar
        double dProgress = (double(i+1)/(*para_hyb).Ncyc) * 100;
        ui->progressBar->setValue(dProgress);
    }
}

void SSA_Solver::Update_amu(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection)
{
    for (int i = 0; i < (*para).M_r; i++)
    {
        // Calculate combination number of reactants i.e., hm
        double a = 1.0;
        for (int j = 0; j < (*para).N_r; j++)
        {
            if ((*reaction_collection)[i].SS[j] == 3)
            {
                a = a * iter->Num_molecules[j] * (iter->Num_molecules[j] - 1.0) * (iter->Num_molecules[j] - 2.0) / 6.0;
            }
            else if ((*reaction_collection)[i].SS[j] == 2)
            {
                a = a * iter->Num_molecules[j] * (iter->Num_molecules[j] - 1.0) / 2.0;
            }
            else if ((*reaction_collection)[i].SS[j] == 1)
            {
                a = a * iter->Num_molecules[j];
            }
            else if ((*reaction_collection)[i].SS[j] == 0)
            {
                a = a * 1.0;
            }
        }
        (*reaction_collection)[i].hmu = a;

        // Calculate the propensity functions by am = cm*hm
        (*reaction_collection)[i].amu = (*reaction_collection)[i].rate * (*reaction_collection)[i].hmu;
    }
}

void SSA_Solver::Reaction_Time_DM(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection)
{
    // Sum a0 if reaction is stochastic reaction
    double a0 = 0.0;
    for (int i = 0; i < (*para).M_r; i++)
    {
        if ((*reaction_collection)[i].D_S == 1)
        {
            a0 = a0 + (*reaction_collection)[i].amu;
        }
    }

    // Reaction time
    double r1 = Basic_Class::random_0_to_1();
    iter->tmin = -log(r1) / a0;

    // Reaction channel
    double r2 = Basic_Class::random_0_to_1();
    double ra0 = r2 * a0;

    double Sum_amu = 0.0;
    int k = 0;
    while (Sum_amu < ra0)
    {
        Sum_amu = Sum_amu + (*reaction_collection)[k].amu;
        k = k + 1;
    }
    /* NOTE!!! c++ bug is here, while loop will no work in certain loop!!!! that is k = 0, so X_min_loc = -1; */
    /* SO we add follow if.... */
    if (k >= 1)
    {
        k = k - 1;
    }
    else
    {
        k = 0;
    }
    iter->X_min_loc = k;

    // No reactants for any reaction happen
    if (a0 < (std::numeric_limits<double>::min)())
    {
        iter->cal_stop = true;
        iter->tmin = 0.0;
        iter->X_min_loc = -1;
    }

    if (iter->cal_stop == false)
    {
        // Time go
        iter->t = iter->t + iter->tmin;

        // Reaction go
        for (int i = 0; i < (*para).N_r; i++)
        {
            iter->Num_molecules[i] = iter->Num_molecules[i] + (*reaction_collection)[iter->X_min_loc].vmu[i];
        }
    }
}

double SSA_Solver::Reaction_Time_DM_ODE(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection, double y_pre[], double a0_t)
{
    // Sum a0 if reaction is stochastic reaction
    // Sum aa if reaction is stochastic or deterministic reaction
    double a0 = 0.0;
    for (int i = 0; i < (*para).M_r; i++)
    {
        if ((*reaction_collection)[i].D_S == 1)
        {
            a0 = a0 + (*reaction_collection)[i].amu;
        }
    }

    a0_t = a0_t + a0 * (*para).h; // 对每轮循环的反应倾向性进行记录, 只用随机反应的趋向性函数*时间步

    double r = Basic_Class::random_0_to_1();

    r = -log(r);

    if (a0_t > r) // SSA happen !!!
    {
        a0_t = 0.0;

        double r1 = Basic_Class::random_0_to_1();
        iter->tmin = -log(r1) / a0;

        double r2 = Basic_Class::random_0_to_1();
        double ra0 = r2 * a0;

        double Sum_amu = 0.0;
        int k = 0;
        while (Sum_amu < ra0)
        {
            if ((*reaction_collection)[k].D_S == 1) // 注意这里的区别！！！非常重要！！！
            {
                Sum_amu = Sum_amu + (*reaction_collection)[k].amu;
            }
            k = k + 1;
        }

        if (k >= 1)
        {
            k = k - 1;
        }
        else
        {
            k = 0;
        }
        iter->X_min_loc = k;

        // No reaction happen
        if (a0 < (std::numeric_limits<double>::min)())
        {
            iter->tmin = 0.0;
            iter->X_min_loc = -1;
            iter->cal_stop = true;
        }

        if (iter->cal_stop == false) // 发生随机反应
        {
            // 反应状态返回ODE计算之前
            iter->t = iter->t - (*para).h; // 时间回退

            for (int k = 0; k <(*para).N_r; k++) // 反应分子回退
            {
                iter->Num_molecules[k] = y_pre[k];
            }

            // Time go
            iter->t = iter->t + iter->tmin;

            // Reaction go
            for (int i = 0; i < (*para).N_r; i++)
            {
                iter->Num_molecules[i] = iter->Num_molecules[i] + (*reaction_collection)[iter->X_min_loc].vmu[i];
            }
        }
    }

    return a0_t;
}

void SSA_Solver::Update_amu_RSSA(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection)
{
    for (int i = 0; i < (*para).M_r; i++)
    {
        double a_low = 1.0;
        double a_high = 1.0;
        for (int j = 0; j < (*para).N_r; j++)
        {
            if ((*reaction_collection)[i].SS[j] == 3)
            {
                a_low = a_low * iter->Num_molecules_low[j] * (iter->Num_molecules_low[j] - 1.0) * (iter->Num_molecules_low[j] - 2.0) / 6.0;
                a_high = a_high * iter->Num_molecules_high[j] * (iter->Num_molecules_high[j] - 1.0) * (iter->Num_molecules_high[j] - 2.0) / 6.0;
            }
            if ((*reaction_collection)[i].SS[j] == 2)
            {
                a_low = a_low * iter->Num_molecules_low[j] * (iter->Num_molecules_low[j] - 1.0) / 2.0;
                a_high = a_high * iter->Num_molecules_high[j] * (iter->Num_molecules_high[j] - 1.0) / 2.0;
            }
            if ((*reaction_collection)[i].SS[j] == 1)
            {
                a_low = a_low * iter->Num_molecules_low[j];
                a_high = a_high * iter->Num_molecules_high[j];
            }
            if ((*reaction_collection)[i].SS[j] == 0)
            {
                a_low = a_low * 1.0;
                a_high = a_high * 1.0;
            }
        }
        (*reaction_collection)[i].hmu_L = a_low;
        (*reaction_collection)[i].hmu_H = a_high;

        // Calculate the propensity functions by am = cm*hm
        (*reaction_collection)[i].amu_L = (*reaction_collection)[i].rate * (*reaction_collection)[i].hmu_L;
        (*reaction_collection)[i].amu_H = (*reaction_collection)[i].rate * (*reaction_collection)[i].hmu_H;
    }
}

void SSA_Solver::Reaction_Time_RSSA(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection)
{
    // Sum a0 if reaction is stochastic reaction
    double a0 = 0.0;
    for (int i = 0; i < (*para).M_r; i++)
    {
        if ((*reaction_collection)[i].D_S == 1)
        {
            a0 = a0 + (*reaction_collection)[i].amu_H;
        }
    }

    if (a0 > (std::numeric_limits<double>::min)())
    {
        bool accepted = true;

        if ((*para).Methods == "RSSA" || (*para).Methods == "tRSSA")
        {
            while (accepted)
            {
                // Reaction type
                double r2 = Basic_Class::random_0_to_1();
                double ra0 = r2 * a0;

                double Sum_amu = 0.0;
                int k = 0;
                while (Sum_amu < ra0)
                {
                    Sum_amu = Sum_amu + (*reaction_collection)[k].amu_H;
                    k = k + 1;
                }
                /* NOTE!!! c++ bug is here, while loop will no work in certain loop!!!! that is k = 0, so X_min_loc = -1; */
                /* SO we add follow if.... */
                if (k >= 1)
                {
                    k = k - 1;
                }
                else
                {
                    k = 0;
                }

                // IF reaction accept
                double r3 = Basic_Class::random_0_to_1();
                if (r3 <= (*reaction_collection)[k].amu_L/(*reaction_collection)[k].amu_H)
                {
                    iter->X_min_loc = k;
                    accepted = false; // Actually we accept this reaction
                    iter->cal_stop = false;
                }
                else
                {
                    SSA_Solver::Update_amu(para, iter, reaction_collection);

                    if (r3 <= (*reaction_collection)[k].amu/(*reaction_collection)[k].amu_H)
                    {
                        iter->X_min_loc = k;
                        accepted = false; // We accept this reaction by another chance
                        iter->cal_stop = false;
                    }
                    else
                    {
                        iter->cal_stop = true;
                    }
                }

                // Reaction time
                double r1 = Basic_Class::random_0_to_1();
                iter->tmin = -log(r1) / a0;

                // Time go no matter whether a reaction happen or not !!!!!
                iter->t = iter->t + iter->tmin;
            }

            if (iter->cal_stop == false)
            {
                for (int i = 0; i < (*para).N_r; i++)
                {
                    iter->Num_molecules[i] = iter->Num_molecules[i] + (*reaction_collection)[iter->X_min_loc].vmu[i];
                }

                // 检查分子数是否超出边界
                for (int i = 0; i < (*para).N_r; i++)
                {
                    if ((*reaction_collection)[iter->X_min_loc].vmu[i] != 0)
                    {
                        if (iter->Num_molecules[i] > iter->Num_molecules_high[i] || iter->Num_molecules[i] < iter->Num_molecules_low[i])
                        {
                            iter->RSSA_update_number = true;
                        }
                    }
                }
            }
        }
    }
}

void SSA_Solver::Update_amu_tRSSA(parameters_CRS * para, iteration_CRS * iter, QVector<reaction> * reaction_collection, int row_count, QList<QList<QVariant>> * excel_list)
{
    for (int i = 0; i < (*para).M_r; i++)
    {
        double mediate = 0.0;
        double rate_L = 0.0;
        double rate_H = 0.0;
        double a_low = 1.0;
        double a_high = 1.0;
        for (int j = 0; j < (*para).N_r; j++)
        {
            if ((*reaction_collection)[i].SS[j] == 3)
            {
                a_low = a_low * iter->Num_molecules_low[j] * (iter->Num_molecules_low[j] - 1.0) * (iter->Num_molecules_low[j] - 2.0) / 6.0;
                a_high = a_high * iter->Num_molecules_high[j] * (iter->Num_molecules_high[j] - 1.0) * (iter->Num_molecules_high[j] - 2.0) / 6.0;
            }
            if ((*reaction_collection)[i].SS[j] == 2)
            {
                a_low = a_low * iter->Num_molecules_low[j] * (iter->Num_molecules_low[j] - 1.0) / 2.0;
                a_high = a_high * iter->Num_molecules_high[j] * (iter->Num_molecules_high[j] - 1.0) / 2.0;
            }
            if ((*reaction_collection)[i].SS[j] == 1)
            {
                a_low = a_low * iter->Num_molecules_low[j];
                a_high = a_high * iter->Num_molecules_high[j];
            }
            if ((*reaction_collection)[i].SS[j] == 0)
            {
                a_low = a_low * 1.0;
                a_high = a_high * 1.0;
            }
        }
        (*reaction_collection)[i].hmu_L = a_low;
        (*reaction_collection)[i].hmu_H = a_high;

        // Read from excel
        bool t_cycle = false;
        int nn1 = 2 * i;
        int nn2 = 2 * i+1;
        for (int var = 0; var < row_count - 1; var++)
        {
            double low_t = (*excel_list).at(var).at(nn1).toDouble();
            double high_t = (*excel_list).at(var+1).at(nn1).toDouble();
            if (iter->t >= low_t && iter->t < high_t)
            {
                (*reaction_collection)[i].rate = (*excel_list).at(var).at(nn2).toDouble();

                rate_L = (*excel_list).at(var).at(nn2).toDouble();
                rate_H = (*excel_list).at(var+1).at(nn2).toDouble();

                if (rate_L > rate_H)
                {
                    mediate = rate_L;
                    rate_L = rate_H;
                    rate_H = mediate;
                }

                t_cycle = true;
            }

            if (t_cycle)
            {
                break;
            }
        }

        if (t_cycle == false)
        {
            rate_L = 0.0;
            rate_H = 0.0;
        }

        // Calculate the propensity functions by am = cm*hm
        (*reaction_collection)[i].amu_L = rate_L * (*reaction_collection)[i].hmu_L;
        (*reaction_collection)[i].amu_H = rate_H * (*reaction_collection)[i].hmu_H;
    }
}
