#ifndef ITERATION_H
#define ITERATION_H

struct iteration
{
    double t_and_tmin[2]; // t_and_tmin[0] for time t; t_and_tmin[1] for minimal time tmin
    double ECMloc;
    double Traction_old;
    double Traction;
    double Vactin;
    double F_K_sum[2]; // F_K_sum[0] for F_sum; F_K_sum[1] for K_sum;

    // Iteration_variable[]:
    // [0]:ECMloc_old; [1]:ECMloc_old1; [2]:ECMloc_old2;
    // [3]:F_sum_old; [4]:F_sum_old1; [5]:F_sum_old2;
    // [6]:K_sum_old; [7]:K_sum_old1; [8]:K_sum_old2;
    double Iteration_variable[9];

    // 注意注意！！！迭代变量里面用QVector会出错！！！！
    double * dint; // Integrin density
    double * kon;
    double Nmyosin_eff; // Myosin number
    double * Averageforce; // Average force on each type integrin
    double * TF; // Traction force on each type of integrins
    double * N_link; // Record the number of each integrin types
    double Ncycle;
    int Active_sites;
};

#endif
