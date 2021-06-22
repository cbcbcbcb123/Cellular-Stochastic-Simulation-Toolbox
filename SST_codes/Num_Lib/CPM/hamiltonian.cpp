#include "Num_Lib/CPM/hamiltonian.h"
#include "Num_Lib/CPM/find_neighborhood.h"

Hamiltonian::Hamiltonian()
{

}

double Hamiltonian::Derta_Adhesion(int x_pix, int y_pix, int target_pixel, int move_pixel, QVector<QVector <int>> * array, QVector<QVector <double>> * J)
{
    QVector <int> UPRL = Find_neighborhood::Von_Neumann_neighborhood(x_pix, y_pix, target_pixel);

    int location = (*array)[target_pixel][3];
    int up = (*array)[UPRL[0]][3];
    int down = (*array)[UPRL[1]][3];
    int right = (*array)[UPRL[2]][3];
    int left = (*array)[UPRL[3]][3];
    double AA1 = Hamiltonian::Adhesion(location, up, down, left, right, J);

    (*array)[target_pixel][3] = (*array)[move_pixel][3]; // 注意！！！非常容易出错！！不能用array！！！

    location = (*array)[target_pixel][3];
    up = (*array)[UPRL[0]][3];
    down = (*array)[UPRL[1]][3];
    right = (*array)[UPRL[2]][3];
    left = (*array)[UPRL[3]][3];

    double AA2 = Hamiltonian::Adhesion(location, up, down, left, right, J);

    return AA2-AA1;
}

double Hamiltonian::Adhesion(int location, int up, int down, int left, int right, QVector<QVector <double>> * J)
{
    double adhesion = 0.0;

    double up_ad = Adhesion_Between_Pixel(location, up, J);

    adhesion = adhesion + up_ad;

    double down_ad = Adhesion_Between_Pixel(location, down, J);

    adhesion = adhesion + down_ad;

    double right_ad = Adhesion_Between_Pixel(location, right, J);

    adhesion = adhesion + right_ad;

    double left_ad = Adhesion_Between_Pixel(location, left, J);

    adhesion = adhesion + left_ad;

    return adhesion;
}

double Hamiltonian::Adhesion_Between_Pixel(int Pixel_A, int Pixel_B, QVector<QVector <double>> * J)
{
    double re = 0.0;


    for (int i = 0; i < (*J).size(); i++)
    {
        for (int j = i; j < (*J).size(); j++) // 注意这里是j = i;
        {
            if ((Pixel_A == i && Pixel_B == j) || (Pixel_A == j && Pixel_B == i))
            {
                re = re + (*J)[i][j];
            }
        }

    }

    return re;
}

double Hamiltonian::Derta_Chemcial_Force(QVector <double> * lamda_Chem_Force, int target_pixel, int move_pixel, QVector<QVector <int>> * array)
{
    // 化学
    double BB2 = -(*lamda_Chem_Force)[0] * ((*array)[target_pixel][1]); // 目前只是x轴坐标，后期可能是不是要专门有一个[4]储存浓度
    double BB1 = -(*lamda_Chem_Force)[0] * ((*array)[move_pixel][1]);

    // 物理
    double BB4 = -(*lamda_Chem_Force)[1] * ((*array)[target_pixel][1]); // 目前只是x轴坐标，后期可能是不是要专门有一个[4]储存浓度
    double BB3 = -(*lamda_Chem_Force)[1] * ((*array)[move_pixel][1]);

    return BB2-BB1+BB4-BB3;
}

double Hamiltonian::Volume_Constraint(int x_pix, int y_pix, int N, QVector <double> * lamda_V, QVector<QVector <int>> * array, QVector <int> * Target_Volume)
{
    QVector <int> Volume(N);
    Volume.clear();
    for (int j = 1; j <= N; j++)
    {
        Volume.push_back(0);
    }

    for (int i = 0; i < x_pix * y_pix; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            if ((*array)[i][3] == j)
            {
                // Notice is [j-1]
                Volume[j-1] = Volume[j-1] + 1;
            }
        }
    }

    double derta_V = 0.0;
    for (int i = 0; i < N; i++)
    {
        derta_V = derta_V + (*lamda_V)[i] * (Volume[i] - (*Target_Volume)[i]) * (Volume[i] - (*Target_Volume)[i]);
    }

    return derta_V;
}

double Hamiltonian::Perimeter_Constraint(int x_pix, int y_pix, int N, QVector <double> * lamda_B, QVector<QVector <int>> * array, QVector <int> * Target_S, QVector<QVector <int>> * Boader)
{
    QVector <int> Perimeter(N);
    Perimeter.clear();
    for (int j = 1; j <= N; j++)
    {
        Perimeter.push_back(0);
    }

    for (int i = 0; i < (*Boader).size(); i++)
    {
        int aa = (*Boader)[i][0]; //要能全覆盖
        int bb = (*Boader)[i][1];

        for (int j = 1; j <= N; j++)
        {
            if ((*array)[aa][3] == j || (*array)[bb][3] == j)
            {
                // Notice is [j-1]
                Perimeter[j-1] = Perimeter[j-1] + 1;
            }
        }
    }

    double derta_B = 0.0;
    for (int i = 0; i < N; i++)
    {
        derta_B = derta_B + (*lamda_B)[i] * (Perimeter[i] - (*Target_S)[i]) * (Perimeter[i] - (*Target_S)[i]);
    }

    return derta_B;
}
