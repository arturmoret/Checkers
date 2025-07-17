#include "Posicio.h"

using namespace std;

//Constructor per defecte
Posicio::Posicio() = default;

//Converteix un string en columna i fila lógiques
Posicio::Posicio(const string& casella)
{
    if (casella.size() == 2 && casella[0] >= 'a' && casella[0] <= 'h' && casella[1] >= '1' && casella[1] <= '8')
    {
        m_columna = casella[0];
        m_fila = casella[1] - '0';
    }
  
}

//Converteix la posició en format text 
string Posicio::toString() const 
{
    char txt[3];
    txt[0] = m_columna;
    txt[1] = '0' + m_fila;
    txt[2] = '\0';
    return string(txt);
}

//Compara si dues posicions son iguals
bool Posicio::operator==(const Posicio& altra) const
{
    return (m_columna == altra.m_columna) && (m_fila == altra.m_fila);
}


