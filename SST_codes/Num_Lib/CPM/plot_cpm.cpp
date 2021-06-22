#include "Num_Lib/CPM/plot_cpm.h"

Plot_CPM::Plot_CPM()
{

}

void Plot_CPM::Plot_Cells(int x_pix, int y_pix, int N, QVector<QVector <int>> * array, QImage * img, QVector <QColor> * tqc)
{
    for (int i = 0; i < x_pix * y_pix; i++)
    {
        if ((*array)[i][3] == 0)
        {
            (*img).setPixelColor((*array)[i][1],(*array)[i][2],QColor(255, 255, 255, 255));
        }

        for (int j = 1; j <= N; j++)
        {
            if ((*array)[i][3] == j)
            {
                (*img).setPixelColor((*array)[i][1],(*array)[i][2],(*tqc)[j-1]);
            }
            if ((*array)[i][3] == j)
            {
                (*img).setPixelColor((*array)[i][1],(*array)[i][2],(*tqc)[j-1]);
            }
        }
    }
}

void Plot_CPM::Plot_Boader(int x_pix, int y_pix, int N, QVector<QVector <int>> * array, QVector<QVector <int>> * Boader, QImage * img, QVector <QColor> * tqc)
{
    // 边界绘图

    for (int i = 0; i < (*Boader).size(); i++)
    {
        int aa = (*Boader)[i][0];
        int bb = (*Boader)[i][1];

        for (int j = 1; j <= N; j++)
        {
            if ((*array)[aa][3] == j)
            {
                (*img).setPixelColor((*array)[aa][1],(*array)[aa][2],(*tqc)[j-1]);
            }
            if ((*array)[bb][3] == j)
            {
                (*img).setPixelColor((*array)[bb][1],(*array)[bb][2],(*tqc)[j-1]);
            }
        }
    }
}
