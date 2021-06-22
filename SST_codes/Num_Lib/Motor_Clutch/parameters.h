#ifndef PARAMETERS_H
#define PARAMETERS_H

struct parameters
{
    /* Clutch Parameters */
    double * Klink;  // Clutch spring stiffness
    int * Bond_type; // 1 for Slip-bond; 2 for Catch-slip-bond
    double * Fcr;    // Reinforcement (unfoled site open) threshold force
    double * dint;   // Base integrin density
    double * dadd;   // Increased kon strengh
    double * dmin;   // Minimal integrin density
    double * dmax;   // Maximal integrin density
    double * kont;   // On-rate dynamics
    double * koff0;  // Slip bond off-rate
    double * kF;     // Slip bond characteristic rupture force
    double * koff1;  // Catch bond parameter
    double * koff2;  // Catch bond parameter
    double * kF1;    // Catch bond parameter
    double * kF2;    // Catch bond parameter

    /* Motor Parameters */
    double V0;          // Actin flow
    double Myosin_loop; // Adhesion-myosin active loop threshold force
    double Myo_dadd;    // Myosin reinforcement strength
    double Myo_min;
    double Myo_max;
    double Nmyosin;     // Myosin number
    double Fstall;      // Myosin stall force

    /* ECM Parameters */
    int ECM_type; // 1 for elasticity; 2 for viscosity; 3 for SLS; 4 for Burgers;
    double E1;
    double E2;
    double R1;
    double R2;

    /* Simulation Parameters */
    int Type_number; // Integrin type number
    int Num_links;   // Fibronectin site or clutch number
    int Ncyc;        // Simulation cycle

    /* Output Results */
    int Out_result; // 0 for actin flow; 1 for traction force; 2 for bounded integrin;
};

#endif
