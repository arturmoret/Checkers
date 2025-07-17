#ifndef JOC_HPP
#define JOC_HPP
#pragma once

#include <iostream>    
#include <string>

#include "info_joc.hpp" //Constants i informació del projecte
#include "Tauler.hpp" //La lógica del tauler
#include "cuaMoviments.hpp" //Per poder gravar i fer replay   

using namespace std;             

class Joc
{
public:

    Joc(); //Crea un joc buit i crida a inicialitza() en el .cpp

    bool inicialitza(ModeJoc mode, const string& fitxerTauler, const string& fitxerMoviments); //Carrega tauler 

    bool actualitza(int mousePosX, int mousePosY, bool mousePremut); //Bucle principal del joc

    void finalitza();

private:
 
    //Estat de la partida
    bool m_acabada = false;   
    ColorFitxa m_guanyador = COLOR_BLANC; //Atribut pel guanyador
    ModeJoc m_mode = MODE_JOC_NORMAL; //Atribut pel mode de joc
    ColorFitxa m_torn = COLOR_BLANC; //Atribut pel color
    bool m_missatgeReplay = false; //Atribut per mostrar el final del replay
    int m_replayIndex = 0; //Índex de la posició actual dins la cua de moviments

    //Selecció de moviments
    bool m_origenSeleccionat = false;                  
    Posicio m_origen;                                               
    Posicio m_destins[MAX_MOVIMENTS_POSSIBLES];                    
    int m_nDestins = 0;                      


    //Model del tauler i l'historial de moviments
    Tauler m_tauler;            
    CuaMoviments m_cuaMoviments;      
    string m_nomFitxerMoviments;

    //Helpers
    void canviaTorn();            
    bool posicioDinsTauler(int x, int y) const; 
    void coordAPosTauler(int x, int y, int& fila, int& col) const;
    bool existeixMoviment(ColorFitxa color) const;     

    //Helpers de la part gràfica
    void pintaFonsITauler() const;
    void dibuixaRessalts() const;
    void dibuixaFitxes() const;
    void dibuixaHUD();

    //Helpers de entrada
    void teclatDesaOCarrega();
    void processaClicModeNormal(int mouseX, int mouseY, bool botObert);
    void processaClicModeReplay(bool botObert);

};

#endif  
