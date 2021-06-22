#include "Num_Lib/Motor_Clutch/mainloop.h"

MainLoop::MainLoop()
{

}

void MainLoop::MC(parameters * para, double Results[][8], double AV_results[])
{
    // 1 for bond states [open or close]; 2 for location; 3 for off-rate
    double (*N_links)[3] = new double [(*para).Num_links][3];
    for (int i = 0; i < (*para).Num_links; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            N_links[i][j] = 0.0; // MUST!!!
        }
    }

    // Iteration variable MUST RESET in here !!!!
    iteration iter = MCM_Initialization::Iteration(para);

    // Main Loop (Note that: iterate "struct variable" in function to call by address using "&")
    for (int ncyc = 0; ncyc < (*para).Ncyc; ncyc++)
    {
        // 1.Calculate integrin-density-dependent on-rate;
        MainLoop::on_rate(para, &iter);

        // 2.Calculate force-dependent off-rate;
        MainLoop::off_rate(para, N_links, &iter);

        // 3.Calculate min time and reaction happen;
        MainLoop::reaction(para, N_links, &iter);

        // 4.Update actin flow and clutch strain;
        MainLoop::actin_flow(para, N_links, &iter);

        // 5.Calculate F_sum and K_sum;
        MainLoop::sum_F_K(para, N_links, &iter);

        // 6.Update ECM location;
        MainLoop::ECM_location(para, &iter);

        // 7.Clear location on open bond;
        MainLoop::clear_location(para, N_links, &iter);

        // 8.Update cell traction;
        MainLoop::traction_force(para, N_links, &iter);

        // 9.Output data
        MCM_Output::Time_variables(para, &iter, Results, AV_results, ncyc);
    }

    MCM_Output::Average_variables(para, &iter, AV_results);

    delete [] N_links;
    delete [] iter.dint;
    delete [] iter.kon;
    delete [] iter.Averageforce;
    delete [] iter.TF;
    delete [] iter.N_link;
}

void MainLoop::on_rate(parameters * para, iteration * iter)
{
    for (int i = 0; i < (*para).Type_number; i++)
    {
        iter->Averageforce[i] = 0.0;
        iter->kon[i] = 0.0;
    }

    // Update the average force
    for (int i = 0; i < (*para).Type_number; i++)
    {
        if (iter->N_link[i] > 0) // Type I integrin at least has one closed bond
        {
            iter->Averageforce[i] = iter->TF[i] / iter->N_link[i];
        }
    }

    // Update the effective on-rate dependent stiffness
    for (int i = 0; i < (*para).Type_number; i++)
    {
        // Case I
        /*
        if (iter->Averageforce[i] >= (*para).Fcr[i])
        {
            iter->kon[i] = (*para).kont[i] + (*para).dadd[i] * (iter->Averageforce[i] - (*para).Fcr[i]);
        }
        else
        {
            iter->kon[i] = (*para).kont[i];
        }
        */

        // Case II
        if (iter->Averageforce[i] >= (*para).Fcr[i] && iter->dint[i] < (*para).dmax[i])
        {
            iter->dint[i] = iter->dint[i] + (*para).dadd[i];
        }

        if (iter->Averageforce[i] < (*para).Fcr[i] && iter->dint[i] > (*para).dmin[i])
        {
            iter->dint[i] = iter->dint[i] - (*para).dadd[i];
        }

        iter->kon[i] = (*para).kont[i] * iter->dint[i];
    }
}

void MainLoop::off_rate(parameters * para, double N_links[][3], iteration * iter)
{
    for (int i = 0; i < (*para).Type_number; i++)
    {
        for (int j = 0; j < (*para).Num_links; j++)
        {
            if (N_links[j][0] > 0.1) // Avoid using double == 0.0
            {
                if ((*para).Bond_type[i] == 1) // Slip bonds
                {
                    double Strain_Slip = (N_links[j][1] - iter->ECMloc);
                    double Force_Slip = Strain_Slip * (*para).Klink[i];
                    N_links[j][2] = (*para).koff0[i] * exp(Force_Slip / (*para).kF[i]);
                }
                if ((*para).Bond_type[i] == 2) // Catch-slip bonds
                {
                    double Strain_Catch = (N_links[j][1] - iter->ECMloc);
                    double Force_Catch = Strain_Catch * (*para).Klink[i];
                    double Catch_Bond_1 = Force_Catch / (*para).kF1[i];
                    double Catch_Bond_2 = -Force_Catch / (*para).kF2[i];
                    N_links[j][2] = (*para).koff1[i] * exp(Catch_Bond_1) + (*para).koff2[i] * exp(Catch_Bond_2);
                }
            }
        }
    }
}

void MainLoop::reaction(parameters * para, double N_links[][3], iteration * iter)
{
    int amu_lattice = (*para).Num_links * ((*para).Type_number + 1);
    double * amu_reaction = new double [amu_lattice];
    // 0.Dynamic array must be initialized
    for (int i = 0; i < amu_lattice; i++)
    {
        amu_reaction [i] = 0.0;
    }

    // 1.on-rate and off-rate
    for (int i = 0; i < (*para).Type_number; i++)
    {
        for (int j = 0; j < (*para).Num_links; j++)
        {
            if (N_links[j][0] < 0.1) // bond state is 0 (open bond)
            {
                amu_reaction[j + i * (*para).Num_links] = iter->kon[i];
            }

            if (N_links[j][0] > 0.1) // bond state is 1 or 2 or others (close bond for type 1 or 2 integrin)
            {
                amu_reaction[j + (*para).Type_number * (*para).Num_links] = N_links[j][2];
            }
        }
    }

    // 2.Min time (SSA_Direct_Method)
    // 2.1 Reaction time
    double a0 = 0.0;
    for (int i = 0; i < amu_lattice; i++) // sum(amu_reaction)
    {
        a0 = a0 + amu_reaction[i];
    }
    double r1 = Basic_Class::random_0_to_1();
    iter->t_and_tmin[1] = -log(r1) / a0; // tmin

    // 2.2 Reaction type
    double r2 = Basic_Class::random_0_to_1();
    double ra0 = r2 * a0;
    double Sum_amu = 0.0;
    int k = 0;
    while (Sum_amu < ra0)
    {
        Sum_amu = Sum_amu + amu_reaction[k];
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
    int X_min_loc = k;

    // 3.Time go on
    iter->t_and_tmin[0] = iter->t_and_tmin[0] + iter->t_and_tmin[1];

    // 4.Reaction happen
    int Which_Integrin_Type = 0;
    int Which_Molecule1 = 0;
    int Which_Molecule2 = 0;

    if (X_min_loc < (*para).Type_number * (*para).Num_links) // Binding reaction [different with Matlab!!]
    {
        Which_Integrin_Type = ceil((X_min_loc + 1) * 1.0 / (*para).Num_links);
        Which_Molecule1 = (X_min_loc + 1) - (*para).Num_links * (Which_Integrin_Type - 1); // Reaction clutch
        N_links[Which_Molecule1 - 1][0] = Which_Integrin_Type;                          // Notice that molecule NO.N is N_links[N-1][...]
    }

    if (X_min_loc >= (*para).Type_number * (*para).Num_links)
    {
        Which_Molecule2 = (X_min_loc + 1) - (*para).Num_links * (*para).Type_number; // Reaction clutch
        N_links[Which_Molecule2 - 1][0] = 0.0;                                 // Notice that molecule NO.30 is N_links[29][...]
    }
    delete [] amu_reaction;
}

void MainLoop::actin_flow(parameters * para, double N_links[][3], iteration * iter)
{
    // 1.Calculate average force on all closed bonds
    double AF = 0.0;
    for (int i = 0; i < (*para).Type_number; i++)
    {
        AF = AF + iter->Averageforce[i];
    }
    AF = AF / (*para).Type_number;

    // 2.Calculate effective myosin number and Update actin flow

    // Case I
    /*
    if (AF >= para.Myosin_loop)
    {
        iter->Nmyosin_eff = para.Nmyosin + para.Myo_dadd * (AF - para.Myosin_loop);
    }
    */

    // Case II
    if (AF >= (*para).Myosin_loop && iter->Nmyosin_eff < (*para).Myo_max)
    {
        iter->Nmyosin_eff = iter->Nmyosin_eff + (*para).Myo_dadd;
    }
    if (AF < (*para).Myosin_loop && iter->Nmyosin_eff > (*para).Myo_min)
    {
        iter->Nmyosin_eff = iter->Nmyosin_eff - (*para).Myo_dadd;
    }

    iter->Vactin = (*para).V0 * (1 - iter->Traction / (iter->Nmyosin_eff * (*para).Fstall));

    // 2.Update clutch length
    for (int i = 0; i < (*para).Num_links; i++)
    {
        if (N_links[i][0] > 0.1) // Close bond
        {
            N_links[i][1] = N_links[i][1] + iter->Vactin * iter->t_and_tmin[1];
        }
    }
}

void MainLoop::sum_F_K(parameters * para, double N_links[][3], iteration * iter)
{
    // 1.Calculate the closed bond number and total strain;
    std::vector<double> sumXi((*para).Type_number); // Note that it must be initialized
    for (int i = 0; i < (*para).Type_number; i++)
    {
        sumXi.push_back(0.0);
    }

    for (int i = 0; i < (*para).Type_number; i++)
    {
        iter->N_link[i] = 0;
        for (int j = 0; j < (*para).Num_links; j++)
        {
            if (N_links[j][0] > i+1-0.1 && N_links[j][0] < i+1+0.1) // NOTE HERE !!!
            {
                sumXi[i] = sumXi[i] + N_links[j][1];
                iter->N_link[i] = iter->N_link[i] + 1;
            }
        }
    }

    // 2.Calculate F_sum and K_sum;
    iter->F_K_sum[0] = 0.0;
    iter->F_K_sum[1] = 0.0;
    for (int i = 0; i < (*para).Type_number; i++)
    {
        iter->F_K_sum[0] = iter->F_K_sum[0] + (*para).Klink[i] * sumXi[i];        // F_sum;
        iter->F_K_sum[1] = iter->F_K_sum[1] + (*para).Klink[i] * iter->N_link[i]; // K_sum;
    }

    sumXi.clear();
}

void MainLoop::ECM_location(parameters * para, iteration * iter)
{
    double p1 = 0.0;
    double p2 = 0.0;
    double q0 = 0.0;
    double q1 = 0.0;
    double q2 = 0.0;

    switch ((*para).ECM_type)
    {
    case 1:
        q0 = (*para).E1;
        break;
    case 2:
        q1 = (*para).R1;
        break;
    case 3:
        p1 = (*para).R1 / (*para).E1;
        q0 = (*para).E2;
        q1 = ((*para).E1 + (*para).E2) * (*para).R1 / (*para).E1;
        break;
    case 4:
        p1 = (*para).R1 / (*para).E1 + (*para).R2 / (*para).E1 + (*para).R2 / (*para).E2;
        p2 = (*para).R1 * (*para).R2 / (*para).E1 / (*para).E2;
        q1 = (*para).R2;
        q2 = (*para).R1 * (*para).R2 / (*para).E1;
        break;
    }

    int sum_N_link = 0;
    for (int i = 0; i < (*para).Type_number; i++)
    {
        sum_N_link = sum_N_link + iter->N_link[i];
    }

    switch ((*para).ECM_type)
    {
    case 1:
        iter->ECMloc = MainLoop::ECM_location_spring(iter->F_K_sum, sum_N_link, q0);
        break;
    case 2:
        iter->ECMloc = MainLoop::ECM_location_dashpot(iter->F_K_sum, sum_N_link, iter->t_and_tmin, iter->Iteration_variable, q1);
        break;
    case 3:
        iter->ECMloc = MainLoop::ECM_location_SLS(iter->F_K_sum, sum_N_link, iter->t_and_tmin, iter->Iteration_variable, q0, q1, p1);
        break;
    case 4:
        iter->ECMloc = MainLoop::ECM_location_Burgers(iter->F_K_sum, sum_N_link, iter->t_and_tmin, iter->Iteration_variable, p1, p2, q1, q2);
        break;
    }
}

double MainLoop::ECM_location_spring(double F_K_sum[], double N_link, double q0)
{
    double ECMloc = 0.0;
    if (N_link > 0)
    {
        ECMloc = F_K_sum[0] / (F_K_sum[1] + q0);
    }
    else
    {
        ECMloc = 0;
    }
    return ECMloc;
}

double MainLoop::ECM_location_dashpot(double F_K_sum[], double N_link, double t_and_tmin[], double Iteration_variable[], double q1)
{
    double ECMloc = 0.0;
    double dt = 0.01; // cycle time for calculating the substrate strain.
    int cc = floor(t_and_tmin[1] / dt);
    double A = 0;

    if (N_link > 0)
    {
        if (cc != 0 )
        {
            for (int i = 0; i < cc; i++)
            {
                A = 1 + dt * F_K_sum[1] / q1;
                ECMloc = Iteration_variable[0] / A + dt / q1 / A * F_K_sum[0];
                Iteration_variable[0] = ECMloc;
            }

            dt = t_and_tmin[1] - cc * dt;
            A = 1 + dt * F_K_sum[1] / q1;
            ECMloc = Iteration_variable[0] / A + dt / q1 / A * F_K_sum[0];
            Iteration_variable[0] = ECMloc;
        }

        if (cc == 0 )
        {
            dt = t_and_tmin[1];
            A = 1 + dt * F_K_sum[1] / q1;
            ECMloc = Iteration_variable[0] / A + dt / q1 / A * F_K_sum[0];
            Iteration_variable[0] = ECMloc;
        }
    }
    else
    {
        ECMloc = 0;
        Iteration_variable[0] = 0;
    }
    return ECMloc;
}

double MainLoop::ECM_location_SLS(double F_K_sum[], double N_link, double t_and_tmin[], double Iteration_variable[], double q0, double q1, double p1)
{
    double ECMloc = 0.0;
    double dt = 0.01; // cycle time for calculating the substrate strain.
    int cc = floor(t_and_tmin[1] / dt);
    double A = 0;
    double B = 0;
    double C = 0;

    if (N_link > 0)
    {
        if (cc != 0)
        {
            for (int i = 0; i < cc; i++)
            {
                A = (p1 * Iteration_variable[6] + q1) / dt;
                B = (1 + p1 / dt) * F_K_sum[0] - p1 / dt * Iteration_variable[3];
                C = q0 + q1 / dt + F_K_sum[1] + F_K_sum[1] * p1 / dt;

                ECMloc = A / C * Iteration_variable[0] + B / C;

                Iteration_variable[0] = ECMloc;
                Iteration_variable[3] = F_K_sum[0];
                Iteration_variable[6] = F_K_sum[1];
            }

            dt = t_and_tmin[1] - cc * dt;
            A = (p1 * Iteration_variable[6] + q1) / dt;
            B = (1 + p1 / dt) * F_K_sum[0] - p1 / dt * Iteration_variable[3];
            C = q0 + q1 / dt + F_K_sum[1] + F_K_sum[1] * p1 / dt;

            ECMloc = A / C * Iteration_variable[0] + B / C;

            Iteration_variable[0] = ECMloc;
            Iteration_variable[3] = F_K_sum[0];
            Iteration_variable[6] = F_K_sum[1];
        }


        if (cc == 0)
        {
            dt = t_and_tmin[1];
            A = (p1 * Iteration_variable[6] + q1) / dt;
            B = (1 + p1 / dt) * F_K_sum[0] - p1 / dt * Iteration_variable[3];
            C = q0 + q1 / dt + F_K_sum[1] + F_K_sum[1] * p1 / dt;

            ECMloc = A / C * Iteration_variable[0] + B / C;

            Iteration_variable[0] = ECMloc;
            Iteration_variable[3] = F_K_sum[0];
            Iteration_variable[6] = F_K_sum[1];
        }

    }
    else
    {
        ECMloc = 0;
        Iteration_variable[0] = 0;
        Iteration_variable[3] = 0;
        Iteration_variable[6] = 0;
    }
    return ECMloc;
}

double MainLoop::ECM_location_Burgers(double F_K_sum[], double N_link, double t_and_tmin[], double Iteration_variable[], double p1, double p2, double q1, double q2)
{
    double ECMloc = 0.0;
    double dt = 0.01; // cycle time for calculating the substrate strain.
    int cc = floor(t_and_tmin[1] / dt);
    double M0 = 0;
    double M1 = 0;
    double M2 = 0;
    double N = 0;

    if (N_link > 0)
    {
        if (cc != 0)
        {
            for (int i = 0; i < cc; i++)
            {
                M0 = F_K_sum[0] * (1 + p1 / dt + p2 / pow(dt, 2)) - Iteration_variable[5] * (p1 / dt + 2 * p2 / pow(dt, 2)) + p2 / pow(dt, 2) * Iteration_variable[4];
                M1 = Iteration_variable[8] * p1 / dt + 2 * Iteration_variable[8] * p2 / pow(dt, 2) + q1 / dt + 2 * q2 / pow(dt, 2);
                M2 = Iteration_variable[7] * p2 / pow(dt, 2) + q2 / pow(dt, 2);
                N = F_K_sum[1] + F_K_sum[1] * p1 / dt + F_K_sum[1] * p2 / pow(dt, 2) + q1 / dt + q2 / pow(dt, 2);

                ECMloc = (M0 + M1 * Iteration_variable[2] - M2 * Iteration_variable[1]) / N;

                Iteration_variable[1] = Iteration_variable[2];
                Iteration_variable[2] = ECMloc;
                Iteration_variable[4] = Iteration_variable[5];
                Iteration_variable[5] = F_K_sum[0];
                Iteration_variable[7] = Iteration_variable[8];
                Iteration_variable[8] = F_K_sum[1];
            }

            dt = t_and_tmin[1] - cc * dt;

            M0 = F_K_sum[0] * (1 + p1 / dt + p2 / pow(dt, 2)) - Iteration_variable[5] * (p1 / dt + 2 * p2 / pow(dt, 2)) + p2 / pow(dt, 2) * Iteration_variable[4];
            M1 = Iteration_variable[8] * p1 / dt + 2 * Iteration_variable[8] * p2 / pow(dt, 2) + q1 / dt + 2 * q2 / pow(dt, 2);
            M2 = Iteration_variable[7] * p2 / pow(dt, 2) + q2 / pow(dt, 2);
            N = F_K_sum[1] + F_K_sum[1] * p1 / dt + F_K_sum[1] * p2 / pow(dt, 2) + q1 / dt + q2 / pow(dt, 2);

            ECMloc = (M0 + M1 * Iteration_variable[2] - M2 * Iteration_variable[1]) / N;

            Iteration_variable[1] = Iteration_variable[2];
            Iteration_variable[2] = ECMloc;
            Iteration_variable[4] = Iteration_variable[5];
            Iteration_variable[5] = F_K_sum[0];
            Iteration_variable[7] = Iteration_variable[8];
            Iteration_variable[8] = F_K_sum[1];
        }

        if (cc == 0)
        {
            dt = t_and_tmin[1];

            M0 = F_K_sum[0] * (1 + p1 / dt + p2 / pow(dt, 2)) - Iteration_variable[5] * (p1 / dt + 2 * p2 / pow(dt, 2)) + p2 / pow(dt, 2) * Iteration_variable[4];
            M1 = Iteration_variable[8] * p1 / dt + 2 * Iteration_variable[8] * p2 / pow(dt, 2) + q1 / dt + 2 * q2 / pow(dt, 2);
            M2 = Iteration_variable[7] * p2 / pow(dt, 2) + q2 / pow(dt, 2);
            N = F_K_sum[1] + F_K_sum[1] * p1 / dt + F_K_sum[1] * p2 / pow(dt, 2) + q1 / dt + q2 / pow(dt, 2);

            ECMloc = (M0 + M1 * Iteration_variable[2] - M2 * Iteration_variable[1]) / N;

            Iteration_variable[1] = Iteration_variable[2];
            Iteration_variable[2] = ECMloc;
            Iteration_variable[4] = Iteration_variable[5];
            Iteration_variable[5] = F_K_sum[0];
            Iteration_variable[7] = Iteration_variable[8];
            Iteration_variable[8] = F_K_sum[1];
        }
    }
    else
    {
        ECMloc = 0;
        Iteration_variable[1] = 0;
        Iteration_variable[2] = 0;
        Iteration_variable[4] = 0;
        Iteration_variable[5] = 0;
        Iteration_variable[7] = 0;
        Iteration_variable[8] = 0;
    }
    return ECMloc;
}

void MainLoop::clear_location(parameters * para, double N_links[][3], iteration * iter)
{
    iter->Active_sites = 0; // Calculate active site number

    for (int i = 0; i < (*para).Num_links; i++)
    {
        if (N_links[i][0] < 0.1) // 尽量不用 ==0.0
        {
            N_links[i][1] = iter->ECMloc; // Off-state clutch has the same position with substrate!!
            N_links[i][2] = 0;
        }

        for (int j = 0; j < (*para).Type_number; j++)
        {
            if (N_links[i][0] > j+1.0-0.1 && N_links[i][0] < j+1.0+0.1)
            {
                if ((*para).Klink[i] * (N_links[i][1] - iter->ECMloc) > (*para).Fcr[j])
                {
                    iter->Active_sites = iter->Active_sites + 1;
                }
            }
        }
    }
}

void MainLoop::traction_force(parameters * para, double N_links[][3], iteration * iter)
{
    iter->Traction = 0;

    for (int j = 0; j < (*para).Type_number; j++)
    {
        iter->TF[j] = 0.0;

        for (int i = 0; i < (*para).Num_links; i++)
        {
            if (N_links[i][0] > j+1.0-0.1 && N_links[i][0] < j+1.0+0.1) // Very important!!! A big BUG here previously
            {
                iter->TF[j] = iter->TF[j] + (*para).Klink[j] * (N_links[i][1] - iter->ECMloc);
            }
        }
        iter->Traction = iter->Traction + iter->TF[j];
    }

    if (iter->Traction > iter->Nmyosin_eff * (*para).Fstall)
    {
        iter->Traction = iter->Nmyosin_eff * (*para).Fstall;
    }
}
