#include "Num_Lib/CRS/ode_solver.h"
#include "UI/hybrid.h"

ODE_Solver::ODE_Solver()
{

}

void ODE_Solver::Time_Independent_ODE(Ui::Hybrid * ui, QVector<struct reaction> * reaction_collection_hyb, iteration_CRS * iter_hyb, parameters_CRS * para_hyb, int Reactant_number, double * t, double ** array)
{
    for (int i = 0; i < (*para_hyb).Ncyc; i++)
    {
        // Record results
        t[i] = (*iter_hyb).t;
        for (int j = 0; j < Reactant_number; j++)
        {
            array[j][i] = (*iter_hyb).Num_molecules[j];
        }

        //ODE_Solver::Euler(para_hyb, iter_hyb, reaction_collection_hyb);

        //ODE_Solver::Runge_Kutta(para_hyb, iter_hyb, reaction_collection_hyb);

        ODE_Solver::Treanor(para_hyb, iter_hyb, reaction_collection_hyb);

        (*iter_hyb).t = (*iter_hyb).t + (*para_hyb).h;

        // Set progress bar
        double dProgress = (double(i+1)/(*para_hyb).Ncyc) * 100;
        ui->progressBar->setValue(dProgress);
    }
}

void ODE_Solver::Time_Dependent_ODE(Ui::Hybrid * ui, QList<QList<QVariant>> * excel_list, int row_count, QVector<struct reaction> * reaction_collection_hyb, iteration_CRS * iter_hyb, parameters_CRS * para_hyb, int Reactant_number, double * t, double ** array)
{
    for (int i = 0; i < (*para_hyb).Ncyc; i++)
    {
        // Record results
        t[i] = (*iter_hyb).t;
        for (int j = 0; j < Reactant_number; j++)
        {
            array[j][i] = (*iter_hyb).Num_molecules[j];
        }

        // Read instantaneous rate from excel
        for (int mm = 0; mm < (*para_hyb).M_r; mm++)
        {
            bool t_cycle = false;
            int nn1 = 2 * mm; // Time change array at Column ID for reaction mm
            int nn2 = 2 * mm+1; // Rate change array at Column ID for reaction mm
            for (int var = 0; var < row_count - 1; var++)
            {
                double low_t = (*excel_list).at(var).at(nn1).toDouble(); // .at(row).at(column)
                double high_t = (*excel_list).at(var+1).at(nn1).toDouble();
                if ((*iter_hyb).t >= low_t && (*iter_hyb).t < high_t)
                {
                    (*reaction_collection_hyb)[mm].rate = (*excel_list).at(var).at(nn2).toDouble();
                    t_cycle = true;
                }
                if (t_cycle) break;
            }
        }

        ODE_Solver::Treanor(para_hyb, iter_hyb, reaction_collection_hyb);

        (*iter_hyb).t = (*iter_hyb).t + (*para_hyb).h;

        // Set progress bar
        double dProgress = (double(i+1)/(*para_hyb).Ncyc) * 100;
        ui->progressBar->setValue(dProgress);
    }
}

void ODE_Solver::Euler(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection)
{
    int n = (*para).N_r;
    // double t = (*para).t; // unused for now
    double h = (*para).h;
    double eps = (*para).eps;

    double * a = new double[n];
    double * b = new double[n];
    double * c = new double[n];
    double * d = new double[n];

    int m = 1;
    double hh = h;
    double p = 1.0 + eps;
    double q = 0.0;
    // double x = t; // unused for now

    double y[n];
    for (int k = 0; k < (*para).N_r; k++)
    {
        y[k] = iter->Num_molecules[k];
    }

    for (int i = 0; i <= n - 1; i++)
    {
        a[i] = y[i];
    }

    while (p >= eps)
    {
        for (int i = 0; i <= n - 1; i++)
        {
            b[i] = y[i];
            y[i] = a[i];
        }

        // 在m步内，由y[n]计算y[n+1]
        for (int j = 0; j <= m - 1; j++)
        {
            for (int i = 0; i <= n - 1; i++)
            {
                c[i] = y[i];
            }

            // x = t + j * hh; // unused for now

            ODE_Solver::Update_amu_ODE(para, iter, reaction_collection, d); // Calculate f(...)

            for (int i = 0; i <= n - 1; i++)
            {
                y[i] = c[i] + hh * d[i];
            }

            // x = t + (j + 1) * hh; // unused for now

            ODE_Solver::Update_amu_ODE(para, iter, reaction_collection, d); // Calculate f(...)

            for (int i = 0; i <= n - 1; i++)
            {
                d[i] = c[i] + hh * d[i];
            }

            for (int i = 0; i <= n - 1; i++)
            {
                y[i] = (y[i] + d[i]) / 2.0;
            }

        }

        // 判断误差是否满足要求
        p = 0.0;
        for (int i = 0; i <= n - 1; i++)
        {
            q = fabs(y[i] - b[i]);

            if (q > p)
            {
                p = q;
            }
        }
        hh = hh / 2.0;
        m = m + m;
    }

    for (int k = 0; k < (*para).N_r; k++)
    {
        iter->Num_molecules[k] = y[k];
    }

    delete[] a;
    delete[] b;
    delete[] c;
    delete[] d;
    return;
}

void ODE_Solver::Runge_Kutta(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection)
{
    int n = (*para).N_r;
    double t = (*para).t;
    double h = (*para).h;
    double eps = (*para).eps;

    int m = 1;
    double hh = h;
    double p = 1.0 + eps;
    double x = t;
    double dt = h / m;
    double q = 0.0;

    double y[n], a[4];

    for (int k = 0; k <(*para).N_r; k++)
    {
        y[k] = iter->Num_molecules[k];
    }

    double * g = new double[n];
    double * b = new double[n];
    double * c = new double[n];
    double * d = new double[n];
    double * e = new double[n];

    for (int i = 0; i <= n - 1; i++)
    {
        c[i] = y[i]; // 将初始y[n]存储在c中，在循环中不会变化
    }

    while (p >= eps)
    {
        a[0] = hh / 2.0; // 时间步
        a[1] = a[0];
        a[2] = hh;
        a[3] = hh;

        for (int i = 0; i <= n - 1; i++)
        {
            g[i] = y[i];
            y[i] = c[i];
        }

        dt = h / m;
        t = x;

        // 在m步内，由y[n]计算y[n+1]
        for (int j = 0; j <= m - 1; j++)
        {
            // Calculate f(...)
            ODE_Solver::Update_amu_ODE(para, iter, reaction_collection, d);

            for (int i = 0; i <= n - 1; i++)
            {
                b[i] = y[i]; // 累积量
                e[i] = y[i]; // K2,K3,K4中的yn
            }

            for (int k = 0; k <= 2; k++)
            {
                for (int i = 0; i <= n - 1; i++)
                {
                    y[i] = e[i] + a[k] * d[i];
                    b[i] = b[i] + a[k + 1] * d[i] / 3.0;
                }

                // Calculate f(...)
                ODE_Solver::Update_amu_ODE(para, iter, reaction_collection, d);

                /* 循环说明
                 for k = 0, y[i] = e[i] + a[k] * d[i] ==> y[i] = y_n + h/2 * K1, where K1 = f(y[n])
                 for k = 0, b[i] = b[i] + a[k + 1] * d[i] / 3.0 ==> b[i] = y_n + h/6 * K1
                 for k = 0, Update_amu_ODE ==> d[i] = K2 = f(y_n + h/2 * K1)

                 for k = 1, y[i] = e[i] + a[k] * d[i] ==> y[i] = y_n + h/2 * K2
                 for k = 1, b[i] = b[i] + a[k + 1] * d[i] / 3.0 ==> b[i] = y_n + h/6 * K1 + h/3 * K2
                 for k = 1, Update_amu_ODE ==> d[i] = K3 = f(y_n + h/2 * K2)

                 for k = 2, y[i] = e[i] + a[k] * d[i] ==> y[i] = y_n + h * K3
                 for k = 2, b[i] = b[i] + a[k + 1] * d[i] / 3.0 ==> b[i] = y_n + h/6 * K1 + h/3 * K2 + h/3 * K3
                 for k = 2, Update_amu_ODE ==> d[i] = K4 = f(y_n + h * K3)
                */
            }

            for (int i = 0; i <= n - 1; i++)
            {
                y[i] = b[i] + hh * d[i] / 6.0; // b[i] + hh * d[i] / 6.0 ==> y_n + h/6 * K1 + h/3 * K2 + h/3 * K3 + h/6 * K4
            }

            t = t + dt;
        }

        // 判断误差是否满足要求
        p = 0.0;
        for (int i = 0; i <= n - 1; i++)
        {
            q = fabs(y[i] - g[i]);

            if (q > p)
            {
                p = q;
            }
        }
        hh = hh / 2.0;
        m = m + m;
    }

    for (int k = 0; k < (*para).N_r; k++)
    {
        iter->Num_molecules[k] = y[k];
    }

    delete[] g;
    delete[] b;
    delete[] c;
    delete[] d;
    delete[] e;
    return;
}

void ODE_Solver::Treanor(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection)
{
    int n = (*para).N_r;
    double t = (*para).t;
    double h = (*para).h;
    double eps = (*para).eps;

    int m = 1;
    double hh = h;
    double pp = 1.0 + eps;
    double x = t;
    double dt = hh / m;
    double dd = 0.0;
    // double s = 0.0; // unused for now
    double aa = 0.0, bb = 0.0, g = 0.0, dy = 0.0, dy1 = 0.0;

    double y[n];
    for (int k = 0; k < (*para).N_r; k++)
    {
        y[k] = iter->Num_molecules[k];
    }

    double * w = new double[4 * n];
    double * q = new double[4 * n];
    double * r = new double[4 * n];
    double * d = new double[n];
    double * p = new double[n];
    double * u = new double[n];
    double * v = new double[n];

    for (int j = 0; j <= n - 1; j++)
    {
        u[j] = y[j];
    }

    while (pp >= eps)
    {
        for (int j = 0; j <= n - 1; j++)
        {
            v[j] = y[j];
            y[j] = u[j];
        }

        t = x;
        dt = hh / m;

        for (int i = 0; i <= m - 1; i++)
        {
            for (int j = 0; j <= n - 1; j++)
            {
                w[j] = y[j];
            }

            ODE_Solver::Update_amu_ODE(para, iter, reaction_collection, d); // Calculate f(...)

            for (int j = 0; j <= n - 1; j++)
            {
                q[j] = d[j];
                y[j] = w[j] + h * d[j] / 2.0;
                w[n + j] = y[j];
            }

            // s = t + h / 2.0; // unused for now

            ODE_Solver::Update_amu_ODE(para, iter, reaction_collection, d); // Calculate f(...)

            for (int j = 0; j <= n - 1; j++)
            {
                q[n + j] = d[j];
                y[j] = w[j] + h * d[j] / 2.0;
                w[n + n + j] = y[j];
            }

            ODE_Solver::Update_amu_ODE(para, iter, reaction_collection, d); // Calculate f(...)

            for (int j = 0; j <= n - 1; j++)
            {
                q[n + n + j] = d[j];
            }

            for (int j = 0; j <= n - 1; j++)
            {
                aa = q[n + n + j] - q[n + j];
                bb = w[n + n + j] - w[n + j];

                if (-aa * bb * h > 0.0)
                {
                    p[j] = -aa / bb;
                    dd = -p[j] * h;
                    r[j] = exp(dd);
                    r[n + j] = (r[j] - 1.0) / dd;
                    r[n + n + j] = (r[n + j] - 1.0) / dd;
                    r[3 * n + j] = (r[n + n + j] - 1.0) / dd;
                }
                else
                {
                    p[j] = 0.0;
                }

                if (p[j] <= 0.0)
                {
                    g = q[n + n + j];
                }
                else
                {
                    g = 2.0 * (q[n + n + j] - q[j]) * r[n + n + j];
                    g = g + (q[j] - q[n + j]) * r[n + j] + q[n + j];
                }

                w[3 * n + j] = w[j] + g * h;
                y[j] = w[3 * n + j];
            }
            // s = t + h; // unused for now

            ODE_Solver::Update_amu_ODE(para, iter, reaction_collection, d); // Calculate f(...)

            for (int j = 0; j <= n - 1; j++)
            {
                q[3 * n + j] = d[j];
            }

            for (int j = 0; j <= n - 1; j++)
            {
                if (p[j] <= 0.0)
                {
                    dy = q[j] + 2.0 * (q[n + j] + q[n + n + j]);
                    dy = (dy + q[n + n + n + j]) * h / 6.0;
                }
                else
                {
                    dy = -3.0 * (q[j] + p[j] * w[j]) + 2.0 * (q[n + j] + p[j] * w[n + j]);
                    dy = dy + 2.0 * (q[n + n + j] + p[j] * w[n + n + j]);
                    dy = dy - (q[n + n + n + j] + p[j] * w[n + n + n + j]);
                    dy = dy * r[n + n + j] + q[j] * r[n + j];
                    dy1 = q[j] - q[n + j] - q[n + n + j] + q[n + n + n + j];
                    dy1 = dy1 + (w[j] - w[n + j] - w[n + n + j] + w[n + n + n + j]) * p[j];
                    dy = (dy + 4.0 * dy1 * r[n + n + n + j]) * h;
                }
                y[j] = w[j] + dy;
            }
            t = t + dt;
        }

        pp = 0.0;
        for (int j = 0; j <= n - 1; j++)
        {
            dd = fabs(y[j] - v[j]);
            if (dd > pp)
            {
                pp = dd;
            }
        }

        h = h / 2.0;
        m = m + m;
    }

    for (int k = 0; k < (*para).N_r; k++)
    {
        iter->Num_molecules[k] = y[k];
    }

    delete[] d;
    delete[] p;
    delete[] w;
    delete[] q;
    delete[] r;
    delete[] u;
    delete[] v;
    return;
}

void ODE_Solver::Update_amu_ODE(parameters_CRS * para, iteration_CRS * iter, QVector<struct reaction> * reaction_collection, double * d)
{
    // !!!数组长度应该是反应物的数目，而不是反应的数目，即<(*para).M_r是不对的!!!
    for (int i = 0; i < (*para).N_r; i++)
    {
        d[i] = 0.0;
    }

    for (int i = 0; i < (*para).M_r; i++)
    {
        double a = 1.0;
        for (int j = 0; j < (*para).N_r; j++)
        {
            if ((*reaction_collection)[i].SS[j] == 3)
            {
                a = a * iter->Num_molecules[j] * (iter->Num_molecules[j] - 1.0) * (iter->Num_molecules[j] - 2.0) / 6.0;
            }
            if ((*reaction_collection)[i].SS[j] == 2)
            {
                a = a * iter->Num_molecules[j] * (iter->Num_molecules[j] - 1.0) / 2.0;
            }
            if ((*reaction_collection)[i].SS[j] == 1)
            {
                a = a * iter->Num_molecules[j];
            }
            if ((*reaction_collection)[i].SS[j] == 0)
            {
                a = a * 1.0;
            }
        }
        (*reaction_collection)[i].hmu = a;
    }

    for (int mm = 0; mm < (*para).M_r; mm++)
    {
        if ((*reaction_collection)[mm].D_S == 0) // If this reaction is ODE
        {
            for (int nn = 0; nn < (*para).N_r; nn++)
            {
                d[nn] = d[nn] + (*reaction_collection)[mm].rate * (*reaction_collection)[mm].hmu * (*reaction_collection)[mm].vmu[nn];
            }
        }
    }
}
