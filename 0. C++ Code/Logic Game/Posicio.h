#ifndef POSICIO_H
#define POSICIO_H
#pragma once

#include <string>
#include <iostream>
#include "info_joc.hpp" //Aquí tenim les constants com NUM_FILES_TAULER o NUM_COLS_TAULER

using namespace std;

constexpr int NUM_COLUMNES_TAULER = NUM_COLS_TAULER;

class Posicio
{
private:
    char m_columna = 'a'; // 'a'..'h'
    int  m_fila = 1; //1..8 (fila visible en el joc)

public:
    
    //Constructors
    Posicio(); // Per defecte "a1"
    Posicio(const string& posicioEnString);   //Per exemple: "c5".

    //Getters
    char getColumna() const { return m_columna; }
    int  getFila() const { return m_fila; }

    //Índex de la matriu (0..7)
    int getFilaIndex()    const { return NUM_FILES_TAULER - m_fila; }
    int getColumnaIndex() const { return m_columna - 'a'; }

    //Setters
    void setColumna(char c) { m_columna = c; }
    void setFila(int f) { m_fila = f; }

    //Conversió a text
    string toString() const;

    //Comparació
    bool operator==(const Posicio& altra) const;
};

#endif   
