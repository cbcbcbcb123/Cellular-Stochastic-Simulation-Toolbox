#ifndef INITIALIZATION_CELL_H
#define INITIALIZATION_CELL_H

#include <QDialog>
#include <QPainter>
#include <QTimer>
#include <cmath>
#include "UI/cpm.h"
#include "Num_Lib/Basic_Class/basic_class.h"

class Initialization_Cell
{
public:
    Initialization_Cell();

    static QVector<QVector <int>> Initialization_Cell_Circular(int x_pix, int y_pix, int x_loc, int y_loc, int R, int N);
};

#endif // INITIALIZATION_CELL_H
