#include "Fitxa.h"

//Constructor per defecte
Fitxa::Fitxa()
{
    m_tipus = TIPUS_EMPTY;
    m_color = COLOR_NEGRE;
}

//Constructor amb paràmetres
Fitxa::Fitxa(TipusFitxa tipus, ColorFitxa color) 
{
    m_tipus = tipus;
    m_color = color;
}

//Actualitza una fitxa normal a dama.
void Fitxa::convertirADama()
{
    if (m_tipus == TIPUS_NORMAL)
    {
        m_tipus = TIPUS_DAMA;
    }
}
