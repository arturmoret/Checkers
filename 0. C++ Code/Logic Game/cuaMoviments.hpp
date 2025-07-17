#ifndef CUA_MOVIMENTS_HPP
#define CUA_MOVIMENTS_HPP
#pragma once

#include <vector>
#include <string>
#include "Moviment.h"

using namespace std;

class CuaMoviments
{
public:

    //Constructor i destructor
    CuaMoviments() = default;
    ~CuaMoviments() = default;

    //Operacions de la pila
    void pushBack(const Moviment& mv); //Afegim un element al final
    bool popFront(Moviment& mv); //Extreiem la de davant
    void clear(); //Esborrem tota la cua
    bool buida() const { return m_dades.empty(); }

    //Consultes
    int size() const { return m_dades.size(); }
    const Moviment& operator[](size_t i) const { return m_dades[i]; }
    const Moviment& getMoviment(size_t i) const { return m_dades[i]; }

    //Guardar i carregar desde archiu
    bool saveToFile(const string& nom) const;
    bool loadFromFile(const string& nom);

private:
    vector<Moviment> m_dades;
};

#endif 
