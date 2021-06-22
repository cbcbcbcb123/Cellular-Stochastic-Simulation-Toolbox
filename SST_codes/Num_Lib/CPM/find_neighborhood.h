#ifndef FIND_NEIGHBORHOOD_H
#define FIND_NEIGHBORHOOD_H

#include "Num_Lib/Basic_Class/basic_class.h"
#include <QVector>

class Find_neighborhood
{
public:
    Find_neighborhood();

    static int Up_neighborhood(int x_pix, int y_pix, int cc);
    static int Right_neighborhood(int x_pix, int y_pix, int cc);
    static int Left_neighborhood(int x_pix, int y_pix, int cc);
    static int Down_neighborhood(int x_pix, int y_pix, int cc);
    static QVector <int> Moore_neighborhood(int x_pix, int y_pix, int cc);
    static QVector <int> Von_Neumann_neighborhood(int x_pix, int y_pix, int cc);
    static QVector<QVector <int>> Find_Boader_Von_Neumann(int x_pix, int y_pix, QVector<QVector <int>> array);
    static QVector<QVector <int>> Find_Boader_Moore(int x_pix, int y_pix, QVector<QVector <int>> array);
};

#endif // FIND_NEIGHBORHOOD_H
