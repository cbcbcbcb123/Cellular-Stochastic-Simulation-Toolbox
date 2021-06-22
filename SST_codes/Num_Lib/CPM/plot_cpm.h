#ifndef PLOT_CPM_H
#define PLOT_CPM_H

#include <QVector>
#include <QColor>
#include <QImage>
#include <QPixmap>

class Plot_CPM
{
public:
    Plot_CPM();

    static void Plot_Cells(int x_pix, int y_pix, int N, QVector<QVector <int>> * array, QImage * img, QVector <QColor> * tqc);

    static void Plot_Boader(int x_pix, int y_pix, int N, QVector<QVector <int>> * array, QVector<QVector <int>> * Boader, QImage * img, QVector <QColor> * tqc);
};

#endif // PLOT_CPM_H
