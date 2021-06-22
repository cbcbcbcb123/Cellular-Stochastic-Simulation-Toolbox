#ifndef HAMILTONIAN_H
#define HAMILTONIAN_H

#include <QVector>

class Hamiltonian
{
public:
    Hamiltonian();

    static double Derta_Adhesion(int x_pix, int y_pix, int target_pixel, int move_pixel, QVector<QVector <int>> * array, QVector<QVector <double>> * J);

    static double Adhesion(int location, int up, int down, int left, int right, QVector<QVector <double>> * J);

    static double Adhesion_Between_Pixel(int Pixel_A, int Pixel_B, QVector<QVector <double>> * J);

    static double Derta_Chemcial_Force(QVector <double> * lamda_Chem_Force, int target_pixel, int move_pixel, QVector<QVector <int>> * array);

    static double Volume_Constraint(int x_pix, int y_pix, int N, QVector <double> * lamda_V, QVector<QVector <int>> * array, QVector <int> * Target_Volume);

    static double Perimeter_Constraint(int x_pix, int y_pix, int N, QVector <double> * lamda_B, QVector<QVector <int>> * array, QVector <int> * Target_S, QVector<QVector <int>> * Boader);
};

#endif // HAMILTONIAN_H
