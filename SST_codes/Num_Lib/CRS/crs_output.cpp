#include "Num_Lib/CRS/crs_output.h"
#include "UI/hybrid.h"

CRS_Output::CRS_Output()
{

}

void CRS_Output::Initialization(parameters_CRS * para_hyb, int Reactant_number, double * t, double ** array)
{
    for (int i = 0; i < (*para_hyb).Ncyc; i++)
    {
        t[i] = 0.0;
    }

    for(int i = 0; i < Reactant_number; i++)
    {
        array[i] = new double [(*para_hyb).Ncyc];
    }
    for (int i = 0; i < Reactant_number; i++)
    {
        for (int j = 0; j < (*para_hyb).Ncyc; j++)
        {
            array[i][j] = 0.0;
        }
    }
}
