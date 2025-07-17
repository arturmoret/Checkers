#ifndef FITXA_H          
#define FITXA_H
#pragma once             

#include <iostream>      

using namespace std;

enum TipusFitxa
{
    TIPUS_NORMAL,
    TIPUS_DAMA,
    TIPUS_EMPTY
};

enum ColorFitxa
{
    COLOR_NEGRE,
    COLOR_BLANC
};

class Fitxa
{
private:
    TipusFitxa  m_tipus = TIPUS_EMPTY;   //Estat inicial: fitxa buida
    ColorFitxa  m_color = COLOR_NEGRE;   //Color per defecte

public:
    //Constructors
    Fitxa(); //Per defecte (buida, negre)
    Fitxa(TipusFitxa tipus, ColorFitxa color); //Amb valors

    //Getters
    TipusFitxa  getTipus() const { return m_tipus; }
    ColorFitxa  getColor() const { return m_color; }

    //Setters
    void setTipus(TipusFitxa t) { m_tipus = t; }
    void setColor(ColorFitxa c) { m_color = c; }

    //Funcions
    void convertirADama();                 
};

#endif 