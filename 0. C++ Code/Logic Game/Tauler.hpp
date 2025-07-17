#ifndef TAULER_HPP
#define TAULER_HPP
#pragma once

#include <vector>
#include "Fitxa.h"
#include "Posicio.h"

using namespace std;

static constexpr int MAX_MOVIMENTS_POSSIBLES = 12;

class Tauler {
public:

    // Constructors i inicialització
    Tauler();
    ~Tauler() = default;
    void inicialitza(const string& nomFitxer);

    //Consultes del estat del tauler 
    const Fitxa& getFitxa(int fila, int col) const { return m_tauler[fila][col]; }
    int calcularMaximaCapturaGlobal(ColorFitxa color, int& filaMaxima, int& columnaMaxima) const;
    int maxCapturaDesDe(const Posicio& origen) const;

    
    //Interfície de consulta de moviments
    void getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const;
    bool mouFitxa(const Posicio& posicioOrigen, const Posicio& posicioDesti);

    // Representació i actualització del estat del tauler
    string toString() const; 

private:

    /* Converteix el vector 8×8 intern a un array 8×8 */
    void vectorAArray(Fitxa dest[NUM_FILES_TAULER][NUM_COLUMNES_TAULER]) const;

    //Estat intern del tauler
    //Estat intern del tauler
    vector<vector<Fitxa>> m_tauler;   // ← OBJECTES (sense * )

    // Validació i precondicions de moviment
    bool validarOrigen(const Posicio& posicioOrigen) const;
    bool validarMoviment(const Posicio& origen, const Posicio& desti) const;
    bool esRival(const Fitxa& origen, const Fitxa& desti) const;

    // Moviment bàsic i coronació
    void moureFitxaSimple(int filaOrigen, int columnaOrigen, int filaDesti, int columnaDesti);
    void coronar(int filaDesti, int columnaDesti);

    // Càlcul de moviments sense captures
    void calculaMovimentsNormals(const Posicio& posicioOrigen, int& totalDestinacions, Posicio destinacions[]) const;
    void calculaMovimentsDama(const Posicio& posicioOrigen, int& totalDestinacions, Posicio destinacions[]) const;
    void afegirCaptures(const Posicio& posicioOrigen, int& totalDestinacions, Posicio destinacions[]) const;

    // Cerca de captures encadenades
    int obtenirSentitFiles(const Fitxa& fitxaOrigen) const;
    void obtenirDesplacamentDiagonal(int indexDireccio, int& desplaFila, int& desplaColumna) const;
    void clonarEstat(const Fitxa src[NUM_FILES_TAULER][NUM_COLUMNES_TAULER], Fitxa dest[NUM_FILES_TAULER][NUM_COLUMNES_TAULER]) const;
    void explorarDireccioCaptura(Fitxa taulerAux[NUM_FILES_TAULER][NUM_COLUMNES_TAULER], int filaActualIndex, int columnaActualIndex, const Fitxa& fitxaOrigen, int indexDireccio, Posicio destinsCaptura[], int& nombreDestins) const;
    void cercaCapturesEncadenades(Fitxa estatTauler[NUM_FILES_TAULER][NUM_COLUMNES_TAULER], int filaActual, int columnaActual, const Fitxa& fitxaOrigen, Posicio destinsCaptura[], int& nDestins) const;

    // Execució de captures
    void executarCapturesNormals(int filaOrigen, int columnaOrigen, int filaDesti, int columnaDesti);
    void executarCapturesDama(int filaOrigen, int columnaOrigen, int filaDesti, int columnaDesti);

    // Salt i captura individual
    bool potFerSalt(const Fitxa estatTauler[NUM_FILES_TAULER][NUM_COLUMNES_TAULER], int filaActual, int columnaActual, int indexDireccio, int& filaInter, int& colInter, int& filaSalt, int& colSalt, const Fitxa& fitxaOrigen) const;

    // Funcions auxiliars de posició
    bool posicioEstaDinsTauler(int fila, int columna) const;
    Posicio creaPosicioATauler(int fila, int columna) const;
    int calculaCapturesEntrePosicions(const Fitxa estatTauler[NUM_FILES_TAULER][NUM_COLUMNES_TAULER], int filaOrigen, int columnaOrigen, int filaDesti, int columnaDesti) const;
    bool trobaCadenaDama(const Fitxa estatTauler[NUM_FILES_TAULER][NUM_COLUMNES_TAULER], int filaActual, int columnaActual, int filaDesti, int columnaDesti, int llistaFilesCaptura[], int llistaColumnesCaptura[], int& nombreCaptura, int llistaFilesSalt[], int llistaColumnesSalt[], int& nombreSalt) const;
};

#endif
