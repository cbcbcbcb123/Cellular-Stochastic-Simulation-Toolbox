#include "Num_Lib/Motor_Clutch/mcm_output.h"

MCM_Output::MCM_Output()
{

}

double MCM_Output::Integrin(parameters * para, iteration * iter)
{
    double total_integrin = 0.0;
    for (int i = 0; i < (*para).Type_number; i++)
    {
        total_integrin = total_integrin + (*iter).dint[i];
    }
    return total_integrin;
}

int MCM_Output::Bounded_receptor(parameters * para, iteration * iter)
{
    int bounded_receptor = 0;
    for (int i = 0; i < (*para).Type_number; i++)
    {
        bounded_receptor = bounded_receptor + (*iter).N_link[i];
    }
    return bounded_receptor;
}

double MCM_Output::Effective_spring(parameters * para, iteration * iter)
{
    double effective_spring = 0.0;
    for (int i = 0; i < (*para).Type_number; i++)
    {
        effective_spring = effective_spring + (*iter).N_link[i] * (*para).Klink[i];
    }
    return effective_spring;
}

void MCM_Output::Cycle_number(iteration * iter)
{
    if ((*iter).ECMloc<=0.001)
    {
        (*iter).Ncycle = (*iter).Ncycle + 1.0;
    }
}

void MCM_Output::Time_variables(parameters * para, iteration * iter, double Results[][8], double AV_results[], int ncyc)
{
    // 9.1 Time-dependent varibles
    Results[ncyc][0] = (*iter).t_and_tmin[0];
    Results[ncyc][1] = (*iter).Vactin;
    Results[ncyc][2] = (*iter).Traction;
    Results[ncyc][3] = MCM_Output::Bounded_receptor(para, iter);
    double bounded_ratio = double(Results[ncyc][3])/(*para).Num_links; // 切记后面要加double转换，否则会等于0
    Results[ncyc][4] = MCM_Output::Integrin(para, iter) * bounded_ratio;
    Results[ncyc][5] = (*iter).ECMloc;
    Results[ncyc][6] = MCM_Output::Effective_spring(para, iter);
    Results[ncyc][7] = (*iter).Active_sites / double((*para).Num_links); // 百分比
    // 9.2 Cycle_number varibles
    MCM_Output::Cycle_number(iter);
    // 9.3 Average varibles
    AV_results[0] = AV_results[0] + (*iter).Traction_old * (*iter).t_and_tmin[1]; // Force
    (*iter).Traction_old = (*iter).Traction;
    AV_results[1] = AV_results[1] + (*iter).Vactin * (*iter).t_and_tmin[1]; // Flow
    AV_results[2] = AV_results[2] + Results[ncyc][4] * (*iter).t_and_tmin[1]; // Adhesion
    AV_results[3] = AV_results[3] + Results[ncyc][6] * (*iter).t_and_tmin[1]; // Effective spring
    AV_results[5] = AV_results[5] + (*iter).Active_sites * (*iter).t_and_tmin[1]; // Active sites
}

void MCM_Output::Average_variables(parameters * para, iteration * iter, double AV_results[])
{
    AV_results[0] = AV_results[0] / (*iter).t_and_tmin[0];
    AV_results[1] = AV_results[1] / (*iter).t_and_tmin[0];
    AV_results[2] = AV_results[2] / (*iter).t_and_tmin[0];
    AV_results[3] = AV_results[3] / (*iter).t_and_tmin[0];
    AV_results[4] = (*iter).t_and_tmin[0] / (*iter).Ncycle;
    AV_results[5] = AV_results[5] / (*iter).t_and_tmin[0] / double((*para).Num_links); // Active site 百分比
}


