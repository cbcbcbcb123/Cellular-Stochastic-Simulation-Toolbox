#include "Num_Lib/Basic_Class/basic_class.h"

Basic_Class::Basic_Class()
{

}

double Basic_Class::random_0_to_1()
{
    double r = rand() * 1.0 / RAND_MAX;
    // [0,1], should exclude 0 and 1 !!!!
    double c1 = std::abs(r - 0.0);
    double c2 = std::abs(r - 1.0);

    while (c1<0.000001 || c2<0.000001)
    {
        r = rand() * 1.0 / RAND_MAX;
        c1 = std::abs(r - 0.0);
        c2 = std::abs(r - 1.0);
    }

    return r;
}
