#include "cuaMoviments.hpp"
#include <fstream>

using namespace std;

//Afegeix un element al final de la cua
void CuaMoviments::pushBack(const Moviment& moviment)
{
    m_dades.push_back(moviment);
}

//Treu el primer element de la cua.
bool CuaMoviments::popFront(Moviment& moviment)
{
    if (m_dades.empty())
    {
        return false;
    }

    moviment = m_dades.front();

    m_dades.erase(m_dades.begin());

    return true;
}

//Buida tota la cua
void CuaMoviments::clear()
{
    m_dades.clear();
}

//Guarda els leements de la cua a un .txt
bool CuaMoviments::saveToFile(const string& nom) const
{
    ofstream fitxer(nom);

    if (!fitxer.is_open())
    {
        return false;
    }

    for (int i = 0; i < int(m_dades.size()); ++i)
    {
        const Moviment& moviment = m_dades[i];

        if (moviment.getNumPosicions() >= 2)
        {
            fitxer << moviment.getPosicio(0).toString() << '-' << moviment.getPosicio(1).toString() << '\n';
        }
    }

    return true;
}


//Carrega una partida desde un fitxer concret
bool CuaMoviments::loadFromFile(const string& nomFitxer)
{
    ifstream fitxer(nomFitxer);

    if (!fitxer.is_open())
    {
        return false;
    }

    m_dades.clear();

    string textPrimeraPosicio = "";
    string textSegonaPosicio = "";
    char caracterActual;

    bool llegintPrimeraPosicio = true;

    while (fitxer.get(caracterActual))
    {
        if (caracterActual == '\n')
        {
            if (textPrimeraPosicio != "" && textSegonaPosicio != "")
            {
                Moviment moviment;
                moviment.afegirPosicio(Posicio(textPrimeraPosicio));
                moviment.afegirPosicio(Posicio(textSegonaPosicio));
                m_dades.push_back(moviment);
            }

            textPrimeraPosicio = "";
            textSegonaPosicio = "";
            llegintPrimeraPosicio = true;
        }

        else if (caracterActual == '-')
        {
            llegintPrimeraPosicio = false;
        }

        else
        {
            if (llegintPrimeraPosicio)
            {
                textPrimeraPosicio += caracterActual;
            }

            else
            {
                textSegonaPosicio += caracterActual;
            }
        }
    }

    // Control per si l��ltima l�nia no acaba en \n
    if (textPrimeraPosicio != "" && textSegonaPosicio != "")
    {
        Moviment moviment;

        moviment.afegirPosicio(Posicio(textPrimeraPosicio));

        moviment.afegirPosicio(Posicio(textSegonaPosicio));

        m_dades.push_back(moviment);
    }

    return true;
}


