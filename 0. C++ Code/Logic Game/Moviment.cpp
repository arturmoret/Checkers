#include "Moviment.h"

//Afegir nova posici� al vector
bool Moviment::afegirPosicio(const Posicio& pos)
{
    m_posicions.push_back(pos);
    return true;
}

//Retornar quantes posicions hi ha al vector
int Moviment::getNumPosicions() const
{
        return m_posicions.size();   
}

//Retorna una posicio concreta o una posicio buida si es inv�lida
Posicio Moviment::getPosicio(int i) const
{
    if (i >= 0 && i < m_posicions.size())
    {
        return m_posicions[i];
    }

    return Posicio();           
}