#ifndef ITERATION_SSA_H
#define ITERATION_SSA_H

#include <QVector>

struct iteration_CRS
{
    QVector<double> Num_molecules; // Molecular number (MN)
    QVector<double> Num_molecules_low; // Lower limit of MN in RSSA
    QVector<double> Num_molecules_high; // Higher limit of MN in RSSA
    double t; // Simulation time
    double tmin; // 化学反应发生对应的最小时间
    int X_min_loc; // 发生反应对应的编号
    bool cal_stop; // 无反应物，退出计算
    bool RSSA_update_number; // RSSA:是否需要更新分子上下限
};

#endif
