#ifndef MOVIMENT_H
#define MOVIMENT_H
#pragma once           

#include <vector>
#include "Posicio.h"

using namespace std;

class Moviment
{
private:
    vector<Posicio> m_posicions; //Vector dinámic que guarda les posicions

public:

    Moviment() = default; //Constructor per defecte 
    
    //Getters
    int getNumPosicions() const;
    Posicio getPosicio(int i) const;

    //Setter
    bool afegirPosicio(const Posicio& pos); //Serveix per afegir una posició al tauler.

};

#endif   
