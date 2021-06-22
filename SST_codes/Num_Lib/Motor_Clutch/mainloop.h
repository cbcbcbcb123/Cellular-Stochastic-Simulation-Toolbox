#ifndef MAINLOOP_H
#define MAINLOOP_H

#include "UI/mcm.h"
#include "Num_Lib/Motor_Clutch/mcm_output.h"
#include "Num_Lib/Motor_Clutch/mcm_initialization.h"
#include "Num_Lib/Basic_Class/basic_class.h"

class MainLoop
{
public:
    MainLoop();

    static void MC(parameters * para, double Results[][8], double AV_results[]);

    static void on_rate(parameters * para, iteration * iter);

    static void off_rate(parameters * para, double N_links[][3], iteration * iter);

    static void reaction(parameters * para, double N_links[][3], iteration * iter);

    static void actin_flow(parameters * para, double N_links[][3], iteration * iter);

    static void sum_F_K(parameters * para, double N_links[][3], iteration * iter);

    static void ECM_location(parameters * para, iteration * iter);

    static double ECM_location_spring(double F_K_sum[], double N_link, double q0);

    static double ECM_location_dashpot(double F_K_sum[], double N_link, double t_and_tmin[], double Iteration_variable[], double q1);

    static double ECM_location_SLS(double F_K_sum[], double N_link, double t_and_tmin[], double Iteration_variable[], double q0, double q1, double p1);

    static double ECM_location_Burgers(double F_K_sum[], double N_link, double t_and_tmin[], double Iteration_variable[], double p1, double p2, double q1, double q2);

    static void clear_location(parameters * para, double N_links[][3], iteration * iter);

    static void traction_force(parameters * para, double N_links[][3], iteration * iter);
};

#endif // MAINLOOP_H
