#include "Num_Lib/CPM/initialization_cell.h"
#include "Num_Lib/CPM/find_neighborhood.h"

Initialization_Cell::Initialization_Cell()
{

}

QVector<QVector <int>> Initialization_Cell::Initialization_Cell_Circular(int x_pix, int y_pix, int x_loc, int y_loc, int R, int N)
{
    QVector<QVector <int>> array(4);
    array.clear(); // QVector定义好 必须clear一下，要不有空余的第一行

    // Initialization
    int kk = 0;
    for(int i = 0; i < x_pix; i++)
    {
        for (int j = 0; j < y_pix; j++)
        {
            array.push_back({kk, i, j, 0});
            kk = kk+1;
        }
    }

    // Set Seed
    for (int k = 1; k <= N; k++)
    {
        double r1 = Basic_Class::random_0_to_1(); // (0,1) 不包括0,1
        double r2 = Basic_Class::random_0_to_1(); // (0,1) 不包括0,1
        double theta = 2 * M_PI * r1;
        double r = R * r2;
        int xx = int(x_loc + r * cos(theta));
        int yy = int(y_loc + r * sin(theta));

        for(int i = 0; i < x_pix * y_pix; i++)
        {
            if (array[i][1] == xx && array[i][2] == yy)
            {
                array[i][3] = k;
            }
        }
    }

    // Expand seed
    QVector<QVector <int>> Boader(2);
    int count = 0;

    while (count < 1000)
    {
        Boader.clear(); // QVector定义好 必须clear一下，要不有空余的第一行
        Boader = Find_neighborhood::Find_Boader_Von_Neumann(x_pix, y_pix, array); // 初始化用Von_Neumann算法比较好

        double r1 = Basic_Class::random_0_to_1(); // (0,1) 不包括0,1
        int rand_chose = ceil(r1 * Boader.size())-1; // 超级大bug，这里应该-1！！！！
        int aa = Boader[rand_chose][0]; //要能全覆盖
        int bb = Boader[rand_chose][1];

        if (array[aa][3]==0)
        {
            int L = sqrt((array[aa][1]-x_loc)*(array[aa][1]-x_loc) + (array[aa][2]-y_loc)*(array[aa][2]-y_loc));

            if (L <= R)
            {
                array[aa][3] = array[bb][3];
            }
            else
            {
                count = count +1;
            }
        }
        else if (array[bb][3]==0)
        {
            int L = sqrt((array[bb][1]-x_loc)*(array[bb][1]-x_loc) + (array[bb][2]-y_loc)*(array[bb][2]-y_loc));

            if (L <= R)
            {
                array[bb][3] = array[aa][3];
            }
            else
            {
                count = count +1;
            }
        }
        else
        {
            // 无空余
            count = count +1;
        }
    }

    return array;
}
