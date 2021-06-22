#include "Num_Lib/CPM/find_neighborhood.h"

Find_neighborhood::Find_neighborhood()
{

}

int Find_neighborhood::Up_neighborhood(int x_pix, int y_pix, int cc)
{
    int check_u = cc - 1;
    if (cc%y_pix == 0) // 边界点
    {
        check_u = cc + y_pix - 1;
    }

    return check_u;
}

int Find_neighborhood::Down_neighborhood(int x_pix, int y_pix, int cc)
{
    int check_d = cc + 1;
    if (check_d%y_pix == 0) // 边界点
    {
        check_d = cc - (y_pix - 1);
    }

    return check_d;
}

int Find_neighborhood::Left_neighborhood(int x_pix, int y_pix, int cc)
{
    int check_l = cc - y_pix;
    if (check_l < 0) // 边界点
    {
        check_l = cc + (x_pix-1)*y_pix;
    }

    return check_l;
}

int Find_neighborhood::Right_neighborhood(int x_pix, int y_pix, int cc)
{
    int check_r = cc + y_pix;
    if (check_r >= x_pix * y_pix) // 边界点
    {
        check_r = cc - (x_pix-1)*y_pix;
    }

    return check_r;
}

QVector <int> Find_neighborhood::Von_Neumann_neighborhood(int x_pix, int y_pix, int cc)
{
    QVector <int> UDRL(4);
    UDRL.clear(); // QVector定义好 必须clear一下，要不有空余的第一行

    int id_1 = Find_neighborhood::Up_neighborhood(x_pix, y_pix, cc);
    int id_2 = Find_neighborhood::Down_neighborhood(x_pix, y_pix, cc);
    int id_3 = Find_neighborhood::Left_neighborhood(x_pix, y_pix, cc);
    int id_4 = Find_neighborhood::Right_neighborhood(x_pix, y_pix, cc);

    UDRL.push_back(id_1);
    UDRL.push_back(id_2);
    UDRL.push_back(id_3);
    UDRL.push_back(id_4);

    return UDRL;
}

QVector <int> Find_neighborhood::Moore_neighborhood(int x_pix, int y_pix, int cc)
{
    QVector <int> UDRL(8);
    UDRL.clear(); // QVector定义好 必须clear一下，要不有空余的第一行

    int id_1 = Find_neighborhood::Up_neighborhood(x_pix, y_pix, cc);
    int id_2 = Find_neighborhood::Down_neighborhood(x_pix, y_pix, cc);
    int id_3 = Find_neighborhood::Left_neighborhood(x_pix, y_pix, cc);
    int id_4 = Find_neighborhood::Right_neighborhood(x_pix, y_pix, cc);

    // 左上
    int id_5 = Find_neighborhood::Left_neighborhood(x_pix, y_pix, id_1);
    // 右上
    int id_6 = Find_neighborhood::Right_neighborhood(x_pix, y_pix, id_1);
    // 左下
    int id_7 = Find_neighborhood::Left_neighborhood(x_pix, y_pix, id_2);
    // 右下
    int id_8 = Find_neighborhood::Right_neighborhood(x_pix, y_pix, id_2);

    UDRL.push_back(id_1);
    UDRL.push_back(id_2);
    UDRL.push_back(id_3);
    UDRL.push_back(id_4);
    UDRL.push_back(id_5);
    UDRL.push_back(id_6);
    UDRL.push_back(id_7);
    UDRL.push_back(id_8);

    return UDRL;
}

QVector<QVector <int>> Find_neighborhood::Find_Boader_Moore(int x_pix, int y_pix, QVector<QVector <int>> array)
{
    QVector<QVector <int>> Boader(2);
    Boader.clear(); // QVector定义好 必须clear一下，要不有空余的第一行

    for (int i = 0; i < x_pix * y_pix; i++)
    {
        int cc = array[i][0];

        int id_1 = Find_neighborhood::Up_neighborhood(x_pix, y_pix, cc);
        int id_2 = Find_neighborhood::Right_neighborhood(x_pix, y_pix, cc);

        if (array[i][3] != array[id_1][3])
        {
            Boader.push_back({i, id_1});
        }
        if (array[i][3] != array[id_2][3])
        {
            Boader.push_back({i, id_2});
        }

        // 右上
        int id_4 = Find_neighborhood::Right_neighborhood(x_pix, y_pix, id_1);
        // 左上
        int id_5 = Find_neighborhood::Left_neighborhood(x_pix, y_pix, id_1);

        if (array[i][3] != array[id_4][3])
        {
            Boader.push_back({i, id_4});
        }
        if (array[i][3] != array[id_5][3])
        {
            Boader.push_back({i, id_5});
        }
    }

    return Boader;
}

QVector<QVector <int>> Find_neighborhood::Find_Boader_Von_Neumann(int x_pix, int y_pix, QVector<QVector <int>> array)
{
    QVector<QVector <int>> Boader(2);
    Boader.clear(); // QVector定义好 必须clear一下，要不有空余的第一行

    for (int i = 0; i < x_pix * y_pix; i++)
    {
        int cc = array[i][0];

        int id_1 = Find_neighborhood::Up_neighborhood(x_pix, y_pix, cc);
        int id_2 = Find_neighborhood::Right_neighborhood(x_pix, y_pix, cc);

        if (array[i][3] != array[id_1][3])
        {
            Boader.push_back({i, id_1});
        }
        if (array[i][3] != array[id_2][3])
        {
            Boader.push_back({i, id_2});
        }
    }

    return Boader;
}

