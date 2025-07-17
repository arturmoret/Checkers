#include "tauler.hpp"
#include <fstream>
#include <iostream>
#include "Moviment.h"
#include "Posicio.h"
#include <vector>

using namespace std;

//Inicialitza un tauler buit amb totes les caselles a “empty”.
Tauler::Tauler()
{
    m_tauler.resize(NUM_FILES_TAULER, std::vector<Fitxa>(NUM_COLUMNES_TAULER, Fitxa()));
}


//Carrega l’estat del tauler llegint un fitxer de text amb les peces i posicions inicials.
void Tauler::inicialitza(const string& nomArxiu)
{
    for (int fila = 0; fila < NUM_FILES_TAULER; fila++)
    {
        for (int columna = 0; columna < NUM_COLUMNES_TAULER; columna++)
        {
            m_tauler[fila][columna] = Fitxa();
        }
    }

    ifstream fitxer(nomArxiu);

    if (!fitxer)
    {
        return;
    }

    char tipus;
    string posicio;

    while (fitxer >> tipus >> posicio)
    {
        Posicio p(posicio);
        int columnaArxiu = p.getColumnaIndex();
        int filaArxiu = p.getFilaIndex();


        TipusFitxa tipusPeça;

        if (tipus == 'D' || tipus == 'R')
        {
            tipusPeça = TIPUS_DAMA;
        }

        else
        {
            tipusPeça = TIPUS_NORMAL;
        }

        ColorFitxa colorPeça;

        if (tipus == 'X' || tipus == 'R')
        {
            colorPeça = COLOR_NEGRE;
        }

        else
        {
            colorPeça = COLOR_BLANC;
        }

        if (posicioEstaDinsTauler(filaArxiu, columnaArxiu))
        {
            m_tauler[filaArxiu][columnaArxiu] = Fitxa(tipusPeça, colorPeça);
        }
    }
}

//Omple l’array amb tots els moviments i salts legals des de origen.
void Tauler::getPosicionsPossibles(const Posicio& posicioOrigen, int& totalDestinacions, Posicio destinacions[]) const
{
    if (!validarOrigen(posicioOrigen))
    {
        totalDestinacions = 0;
        return;
    }

    totalDestinacions = 0;

    int columnaOrigenIndex = posicioOrigen.getColumnaIndex();
    int filaOrigenIndex = posicioOrigen.getFilaIndex();

    TipusFitxa tipusFitxaOrigen = m_tauler[filaOrigenIndex][columnaOrigenIndex].getTipus();

    if (tipusFitxaOrigen == TIPUS_NORMAL)
    {
        calculaMovimentsNormals(posicioOrigen, totalDestinacions, destinacions);
    }
    else
    {
        calculaMovimentsDama(posicioOrigen, totalDestinacions, destinacions);
    }

    afegirCaptures(posicioOrigen, totalDestinacions, destinacions);
}


//Comprova que la posició origen estigui dins del tauler i que hi hagi efectivament una fitxa allà (no buida).
bool Tauler::validarOrigen(const Posicio& posicioOrigen) const
{
    int columnaOrigenIndex = posicioOrigen.getColumnaIndex();
    int filaOrigenIndex = posicioOrigen.getFilaIndex();

    if (!posicioEstaDinsTauler(filaOrigenIndex, columnaOrigenIndex))
    {
        return false;
    }

    if (m_tauler[filaOrigenIndex][columnaOrigenIndex].getTipus() == TIPUS_EMPTY)
    {
        return false;
    }

    return true;
}

//Afegeix als possibles moviments totes les caselles lliures on pot anar una fitxa normal, segons el seu sentit
void Tauler::calculaMovimentsNormals(const Posicio& posicioOrigen, int& totalDestinacions, Posicio destinacions[]) const
{
    int columnaOrigenIndex = posicioOrigen.getColumnaIndex();
    int filaOrigenIndex = posicioOrigen.getFilaIndex();

    int sentitFiles = obtenirSentitFiles(m_tauler[filaOrigenIndex][columnaOrigenIndex]);

    for (int desplaçamentColumna = -1; desplaçamentColumna <= 1; desplaçamentColumna = desplaçamentColumna + 2)
    {
        int filaNova = filaOrigenIndex + sentitFiles;
        int columnaNova = columnaOrigenIndex + desplaçamentColumna;

        if (posicioEstaDinsTauler(filaNova, columnaNova))
        {
            if (m_tauler[filaNova][columnaNova].getTipus() == TIPUS_EMPTY)
            {
                if (totalDestinacions < MAX_MOVIMENTS_POSSIBLES)
                {
                    Posicio posicioNova = creaPosicioATauler(filaNova, columnaNova);
                    destinacions[totalDestinacions] = posicioNova;
                    totalDestinacions++;
                }
            }
        }
    }
}

//Afegeix als possibles moviments totes les caselles diagonals lliures on pot moure’s una dama.
void Tauler::calculaMovimentsDama(const Posicio& posicioOrigen, int& totalDestinacions, Posicio destinacions[]) const
{
    int columnaOrigenIndex = posicioOrigen.getColumnaIndex();
    int filaOrigenIndex = posicioOrigen.getFilaIndex();

    for (int indexDireccio = 0; indexDireccio < 4; indexDireccio++)
    {
        int desplaFila;
        int desplaColumna;

        obtenirDesplacamentDiagonal(indexDireccio, desplaFila, desplaColumna);
        int filaNova = filaOrigenIndex + desplaFila;
        int columnaNova = columnaOrigenIndex + desplaColumna;
        while (posicioEstaDinsTauler(filaNova, columnaNova))
        {
            if (m_tauler[filaNova][columnaNova].getTipus() != TIPUS_EMPTY) { break; }

            if (totalDestinacions >= MAX_MOVIMENTS_POSSIBLES) { break; }

            destinacions[totalDestinacions++] = creaPosicioATauler(filaNova, columnaNova);
            filaNova += desplaFila;
            columnaNova += desplaColumna;
        }
    }
}



//Completa la llista de destinacions amb tots els salts possibles (incloent els encadenats) cridant després els helpers de recursivitat.
void Tauler::afegirCaptures(const Posicio& posicioOrigen, int& totalDestinacions, Posicio destinacions[]) const
{
    int columnaOrigenIndex = posicioOrigen.getColumnaIndex();
    int filaOrigenIndex = posicioOrigen.getFilaIndex();

    Posicio llistaCaptura[MAX_MOVIMENTS_POSSIBLES];
    int nombreCaptura = 0;

    Fitxa aux[NUM_FILES_TAULER][NUM_COLUMNES_TAULER];
    vectorAArray(aux);                                // helper ja implementat
    Fitxa estatTemporal[NUM_FILES_TAULER][NUM_COLUMNES_TAULER];
    clonarEstat(aux, estatTemporal);                  // ara rep un array vàlid


    cercaCapturesEncadenades(estatTemporal, filaOrigenIndex, columnaOrigenIndex, m_tauler[filaOrigenIndex][columnaOrigenIndex], llistaCaptura, nombreCaptura);

    for (int indexCaptura = 0; indexCaptura < nombreCaptura; indexCaptura++)
    {
        bool trobat = false;
        for (int indexDestins = 0; indexDestins < totalDestinacions; indexDestins++)
        {
            if (destinacions[indexDestins] == llistaCaptura[indexCaptura])
            {
                trobat = true;
                break;
            }
        }

        if (!trobat)
        {
            if (totalDestinacions < MAX_MOVIMENTS_POSSIBLES)
            {
                destinacions[totalDestinacions] = llistaCaptura[indexCaptura];
                totalDestinacions++;
            }
        }
    }
}

//Executa el moviment de la fitxa d’origen a destí, aplicant captures i coronacions si escau; retorna true si el moviment era vàlid.
bool Tauler::mouFitxa(const Posicio& posicioOrigen, const Posicio& posicioDesti)
{
    if (!validarMoviment(posicioOrigen, posicioDesti))
    {
        return false;
    }

    int columnaOrigenIndex = posicioOrigen.getColumnaIndex();
    int filaOrigenIndex = posicioOrigen.getFilaIndex();

    int columnaDestinacioIndex = posicioDesti.getColumnaIndex();
    int filaDestinacioIndex = posicioDesti.getFilaIndex();

    ColorFitxa colorFitxaOrigen = m_tauler[filaOrigenIndex][columnaOrigenIndex].getColor();

    int filaMaximaCapturaIndex = -1;
    int columnaMaximaCapturaIndex = -1;
    int maximaCaptura = calcularMaximaCapturaGlobal(colorFitxaOrigen, filaMaximaCapturaIndex, columnaMaximaCapturaIndex);



    Fitxa aux[NUM_FILES_TAULER][NUM_COLUMNES_TAULER];
    vectorAArray(aux);
    int capturesReals = calculaCapturesEntrePosicions(aux, filaOrigenIndex, columnaOrigenIndex, filaDestinacioIndex, columnaDestinacioIndex);


    if (capturesReals > 0)
    {
        if (m_tauler[filaOrigenIndex][columnaOrigenIndex].getTipus() == TIPUS_DAMA)
        {
            executarCapturesDama(filaOrigenIndex, columnaOrigenIndex, filaDestinacioIndex, columnaDestinacioIndex);
        }
        else
        {
            executarCapturesNormals(filaOrigenIndex, columnaOrigenIndex, filaDestinacioIndex, columnaDestinacioIndex);
        }
    }

    moureFitxaSimple(filaOrigenIndex, columnaOrigenIndex, filaDestinacioIndex, columnaDestinacioIndex);
    coronar(filaDestinacioIndex, columnaDestinacioIndex);

    if (maximaCaptura > 0 && capturesReals < maximaCaptura)
    {
        if (filaMaximaCapturaIndex == filaOrigenIndex && columnaMaximaCapturaIndex == columnaOrigenIndex)
        {
            m_tauler[filaDestinacioIndex][columnaDestinacioIndex] = Fitxa();
        }
        else
        {
            m_tauler[filaMaximaCapturaIndex][columnaMaximaCapturaIndex] = Fitxa();
        }
    }


    return true;
}

//Verifica que el destí proposat és un dels moviments legals calculats per a la fitxa a origen.
bool Tauler::validarMoviment(const Posicio& posicioOrigen, const Posicio& posicioDesti) const
{
    int columnaOrigenIndex = posicioOrigen.getColumnaIndex();
    int filaOrigenIndex = posicioOrigen.getFilaIndex();

    int columnaDestinacioIndex = posicioDesti.getColumnaIndex();
    int filaDestinacioIndex = posicioDesti.getFilaIndex();


    if (!validarOrigen(posicioOrigen))
    {
        return false;
    }

    if (!posicioEstaDinsTauler(filaDestinacioIndex, columnaDestinacioIndex))
    {
        return false;
    }


    Posicio posicionsOpcions[MAX_MOVIMENTS_POSSIBLES];
    int nombreOpcions = 0;
    getPosicionsPossibles(posicioOrigen, nombreOpcions, posicionsOpcions);

    for (int indexOpcio = 0; indexOpcio < nombreOpcions; indexOpcio++)
    {
        if (posicionsOpcions[indexOpcio] == posicioDesti)
        {
            return true;
        }
    }

    return false;
}

// Cusca entre totes les fitxes d’un mateix color quina pot fer la seqüència de captures més llarga i en retorna la posició (filaMaxima, columnaMaxima) i el nombre de peces capturables.
int Tauler::calcularMaximaCapturaGlobal(ColorFitxa colorFitxa, int& filaMaximaIndex, int& columnaMaximaIndex) const
{
    int maximaCaptura = 0;
    filaMaximaIndex = -1;
    columnaMaximaIndex = -1;

    for (int filaIndex = 0; filaIndex < NUM_FILES_TAULER; filaIndex++)
    {
        for (int columnaIndex = 0; columnaIndex < NUM_COLUMNES_TAULER; columnaIndex++)
        {
            if (m_tauler[filaIndex][columnaIndex].getColor() != colorFitxa)
            {
                continue;
            }

            Posicio posicioOrigen = creaPosicioATauler(filaIndex, columnaIndex);
            Posicio opcionsDestinacio[MAX_MOVIMENTS_POSSIBLES];

            int nombreOpcions = 0;
            getPosicionsPossibles(posicioOrigen, nombreOpcions, opcionsDestinacio);

            for (int indexOpcio = 0; indexOpcio < nombreOpcions; indexOpcio++)
            {
                int filaDestiIndex = opcionsDestinacio[indexOpcio].getFilaIndex();
                int columnaDestiIndex = opcionsDestinacio[indexOpcio].getColumnaIndex();

                Fitxa aux[NUM_FILES_TAULER][NUM_COLUMNES_TAULER];
                vectorAArray(aux);


                int capturesActuals =
                    calculaCapturesEntrePosicions(aux, filaIndex, columnaIndex, filaDestiIndex, columnaDestiIndex);


                if (capturesActuals > maximaCaptura)
                {
                    maximaCaptura = capturesActuals;
                    filaMaximaIndex = filaIndex;
                    columnaMaximaIndex = columnaIndex;
                }
            }
        }
    }

    return maximaCaptura;
}

//Recorre les diagonals entre l’origen i el destí d’una fitxa normal i va buidant qualsevol peça capturada fins arribar a la casella final
void Tauler::executarCapturesNormals(int filaOrigen, int columnaOrigen, int filaDesti, int columnaDesti)
{
    int filesCaptura[MAX_MOVIMENTS_POSSIBLES];
    int columnesCaptura[MAX_MOVIMENTS_POSSIBLES];
    int nombreCaptura = 0;

    int filesSalt[MAX_MOVIMENTS_POSSIBLES];
    int columnesSalt[MAX_MOVIMENTS_POSSIBLES];
    int nombreSalt = 0;

    Fitxa taulerAux[NUM_FILES_TAULER][NUM_COLUMNES_TAULER];
    vectorAArray(taulerAux);

    bool trobat = trobaCadenaDama(taulerAux, filaOrigen, columnaOrigen, filaDesti, columnaDesti, filesCaptura, columnesCaptura, nombreCaptura, filesSalt, columnesSalt, nombreSalt);

    if (!trobat)
    {
        return;
    }


    for (int i = 0; i < nombreCaptura; ++i)
    {
        int f = filesCaptura[i];
        int c = columnesCaptura[i];

        m_tauler[f][c] = Fitxa();
    }
}



//Elimina del tauler totes les peces capturades durant un moviment de dama, utilitzant els índexs calculats per la funció trobaCadenaDama
void Tauler::executarCapturesDama(int filaOrigenIndex, int columnaOrigenIndex, int filaDestiIndex, int columnaDestiIndex)
{
    int filesCapturaIndex[MAX_MOVIMENTS_POSSIBLES];
    int columnesCapturaIndex[MAX_MOVIMENTS_POSSIBLES];
    int nombreCaptura = 0;

    int filesSaltIndex[MAX_MOVIMENTS_POSSIBLES];
    int columnesSaltIndex[MAX_MOVIMENTS_POSSIBLES];
    int nombreSalt = 0;

    Fitxa aux[NUM_FILES_TAULER][NUM_COLUMNES_TAULER];
    vectorAArray(aux);
    trobaCadenaDama(aux, filaOrigenIndex, columnaOrigenIndex, filaDestiIndex, columnaDestiIndex, filesCapturaIndex, columnesCapturaIndex, nombreCaptura, filesSaltIndex, columnesSaltIndex, nombreSalt);

    for (int indexCaptura = 0; indexCaptura < nombreCaptura; indexCaptura++)
    {
        m_tauler[filesCapturaIndex[indexCaptura]][columnesCapturaIndex[indexCaptura]] = Fitxa();
    }
}

//Mou la fitxa d’una casella a una altra sense eliminar cap peça pel camí.
void Tauler::moureFitxaSimple(int filaOrigenIndex, int columnaOrigenIndex, int filaDestiIndex, int columnaDestiIndex)
{
    Fitxa fitxaOrigen = m_tauler[filaOrigenIndex][columnaOrigenIndex];
    m_tauler[filaDestiIndex][columnaDestiIndex] = fitxaOrigen;
    m_tauler[filaOrigenIndex][columnaOrigenIndex] = Fitxa();
}

//Si una fitxa normal arriba a la primera o última fila, la converteix en dama.
void Tauler::coronar(int filaDestiIndex, int columnaDestiIndex)
{
    Fitxa& fitxaDesti = m_tauler[filaDestiIndex][columnaDestiIndex];
    if (fitxaDesti.getTipus() == TIPUS_NORMAL)
    {
        bool esExtrem;
        ColorFitxa colorFitxa = fitxaDesti.getColor();
        if (colorFitxa == COLOR_BLANC)
        {
            if (filaDestiIndex == 0)
            {
                esExtrem = true;
            }
            else
            {
                esExtrem = false;
            }
        }
        else
        {
            if (filaDestiIndex == NUM_FILES_TAULER - 1)
            {
                esExtrem = true;
            }
            else
            {
                esExtrem = false;
            }
        }
        if (esExtrem)
        {
            fitxaDesti.convertirADama();
        }
    }
}


//Construeix i retorna una cadena ASCII que representa l’estat actual del tauler 8x8
string Tauler::toString() const
{
    string sortida = "";
    int numero = NUM_FILES_TAULER;

    for (int fila = 0; fila < NUM_FILES_TAULER; fila++)
    {
        sortida += '0' + numero;
        sortida += ':';
        sortida += ' ';

        for (int columna = 0; columna < NUM_COLUMNES_TAULER; columna++)
        {
            char car = '_';
            const Fitxa& fitxaActual = m_tauler[fila][columna];

            if (fitxaActual.getTipus() == TIPUS_NORMAL)
            {
                if (fitxaActual.getColor() == COLOR_BLANC)
                {
                    car = 'O';
                }
                else
                {
                    car = 'X';
                }
            }
            else if (fitxaActual.getTipus() == TIPUS_DAMA)
            {
                if (fitxaActual.getColor() == COLOR_BLANC)
                {
                    car = 'D';
                }
                else
                {
                    car = 'R';
                }
            }

            sortida += car;
            if (columna < NUM_COLUMNES_TAULER - 1)
            {
                sortida += ' ';
            }
        }

        sortida += '\n';
        numero--;
    }

    sortida += "   a b c d e f g h";

    return sortida;
}

// A PARTIR D'AQUÍ COMENÇEN LES FUNCIONS AUXILIARS //

//Comprova si unes coordenades de matriu estan dins l’interval vàlid [0,7].
bool Tauler::posicioEstaDinsTauler(int fila, int columna) const
{
    return (fila >= 0 && fila < NUM_FILES_TAULER) && (columna >= 0 && columna < NUM_COLUMNES_TAULER);
}

void Tauler::vectorAArray(Fitxa dest[NUM_FILES_TAULER][NUM_COLUMNES_TAULER]) const
{
    for (int f = 0; f < NUM_FILES_TAULER; ++f)
        for (int c = 0; c < NUM_COLUMNES_TAULER; ++c)
            dest[f][c] = m_tauler[f][c];
}

//Converteix uns índexs de matriu en un objecte Posicio amb notació “a1”–“h8”.

Posicio Tauler::creaPosicioATauler(int fila, int columna) const
{
    char textPosicio[3];
    textPosicio[0] = 'a' + columna;
    textPosicio[1] = '0' + (NUM_FILES_TAULER - fila);
    textPosicio[2] = '\0';
    return Posicio(string(textPosicio));
}

//Fa la recerca recursiva de tots els salts en totes les direccions, afegint-les a l’array.
void Tauler::cercaCapturesEncadenades(Fitxa taulerAux[NUM_FILES_TAULER][NUM_COLUMNES_TAULER], int filaActualIndex, int columnaActualIndex, const Fitxa& fitxaOrigen, Posicio destinsCaptura[], int& nombreDestins) const
{
    for (int direccioIndex = 0; direccioIndex < 4; direccioIndex++)
    {
        explorarDireccioCaptura(taulerAux, filaActualIndex, columnaActualIndex, fitxaOrigen, direccioIndex, destinsCaptura, nombreDestins);
    }
}

//Retorna + 1, –1 o 0 segons el color i tipus de fitxa, per saber en quin sentit fa el “salt”.
int Tauler::obtenirSentitFiles(const Fitxa& fitxaOrigen) const
{
    if (fitxaOrigen.getTipus() == TIPUS_NORMAL)
    {
        ColorFitxa colorFitxa = fitxaOrigen.getColor();
        if (colorFitxa == COLOR_BLANC)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }

    return 0;
}

//Calcula el desplaçament en files i columnes per moure’s en una direcció diagonal específica del tauler
void Tauler::obtenirDesplacamentDiagonal(int indexDireccio, int& desplaFila, int& desplaColumna) const
{
    switch (indexDireccio)
    {

    case 0:
    {
        desplaFila = -1;
        desplaColumna = -1;
        break;
    }

    case 1:
    {
        desplaFila = -1;
        desplaColumna = 1;
        break;
    }

    case 2:
    {
        desplaFila = 1;
        desplaColumna = -1;
        break;
    }

    default:
    {
        desplaFila = 1;
        desplaColumna = 1;
        break;
    }
    }
}

//Fa una còpia completa de l’estat del tauler en un altre array, per poder simular moviments sense modificar l’original.
void Tauler::clonarEstat(const Fitxa src[NUM_FILES_TAULER][NUM_COLUMNES_TAULER], Fitxa dest[NUM_FILES_TAULER][NUM_COLUMNES_TAULER]) const
{
    for (int filaIndex = 0; filaIndex < NUM_FILES_TAULER; filaIndex++)
    {
        for (int columnaIndex = 0; columnaIndex < NUM_COLUMNES_TAULER; columnaIndex++)
        {
            dest[filaIndex][columnaIndex] = src[filaIndex][columnaIndex];
        }
    }
}

//Per a una direcció diagonal concreta, identifica un possible salt, l’afegeix i continúa la recursió.
bool Tauler::esRival(const Fitxa& origen, const Fitxa& desti) const
{
    return (desti.getTipus() != TIPUS_EMPTY) && (desti.getColor() != origen.getColor());
}

void Tauler::explorarDireccioCaptura(Fitxa taulerAux[NUM_FILES_TAULER][NUM_COLUMNES_TAULER], int filaActualIndex, int columnaActualIndex, const Fitxa& fitxaOrigen, int indexDireccio, Posicio destinsCaptura[], int& nombreDestins) const
{
    int desplaFilaIndex;
    int desplaColumnaIndex;
    obtenirDesplacamentDiagonal(indexDireccio, desplaFilaIndex, desplaColumnaIndex);

    int sentitFilesIndex = obtenirSentitFiles(fitxaOrigen);

    if (fitxaOrigen.getTipus() == TIPUS_NORMAL)
    {
        if (desplaFilaIndex != sentitFilesIndex) { return; }
    }

    int filaMitjaIndex = filaActualIndex + desplaFilaIndex;
    int columnaMitjaIndex = columnaActualIndex + desplaColumnaIndex;

    if (fitxaOrigen.getTipus() == TIPUS_DAMA)
    {
        while (posicioEstaDinsTauler(filaMitjaIndex, columnaMitjaIndex) && taulerAux[filaMitjaIndex][columnaMitjaIndex].getTipus() == TIPUS_EMPTY)
        {
            filaMitjaIndex += desplaFilaIndex;
            columnaMitjaIndex += desplaColumnaIndex;
        }
    }
    int filaSaltIndex = filaMitjaIndex + desplaFilaIndex;
    int columnaSaltIndex = columnaMitjaIndex + desplaColumnaIndex;

    if (!posicioEstaDinsTauler(filaMitjaIndex, columnaMitjaIndex)) { return; }

    if (!posicioEstaDinsTauler(filaSaltIndex, columnaSaltIndex)) { return; }

    if (taulerAux[filaMitjaIndex][columnaMitjaIndex].getTipus() == TIPUS_EMPTY) { return; }

    if (!esRival(fitxaOrigen, taulerAux[filaMitjaIndex][columnaMitjaIndex])) { return; }

    if (taulerAux[filaSaltIndex][columnaSaltIndex].getTipus() != TIPUS_EMPTY) { return; }

    if (nombreDestins < MAX_MOVIMENTS_POSSIBLES)
    {
        destinsCaptura[nombreDestins++] = creaPosicioATauler(filaSaltIndex, columnaSaltIndex);
    }

    Fitxa estatNou[NUM_FILES_TAULER][NUM_COLUMNES_TAULER];
    clonarEstat(taulerAux, estatNou);

    estatNou[filaActualIndex][columnaActualIndex] = Fitxa();
    estatNou[filaMitjaIndex][columnaMitjaIndex] = Fitxa();
    estatNou[filaSaltIndex][columnaSaltIndex] = fitxaOrigen;

    cercaCapturesEncadenades(estatNou, filaSaltIndex, columnaSaltIndex, fitxaOrigen, destinsCaptura, nombreDestins);
}



//Recorre la trajectòria entre origen i destí comptant peces no buides, per mesurar quantes es capturarien en un salt.
int Tauler::calculaCapturesEntrePosicions(const Fitxa estatTauler[NUM_FILES_TAULER][NUM_COLUMNES_TAULER], int filaOrigen, int columnaOrigen, int filaDesti, int columnaDesti) const
{
    int sentitFila;

    if (filaDesti > filaOrigen)
    {
        sentitFila = 1;
    }

    else
    {
        sentitFila = -1;
    }

    int sentitColumna;

    if (columnaDesti > columnaOrigen)
    {
        sentitColumna = 1;
    }

    else
    {
        sentitColumna = -1;
    }

    int filaActual = filaOrigen + sentitFila;
    int columnaActual = columnaOrigen + sentitColumna;
    int totalCaptures = 0;

    while (filaActual != filaDesti && columnaActual != columnaDesti)
    {
        if (estatTauler[filaActual][columnaActual].getTipus() != TIPUS_EMPTY)
        {
            totalCaptures++;
        }

        filaActual += sentitFila;
        columnaActual += sentitColumna;
    }

    return totalCaptures;
}

//Calcula la seqüència de salts necessària perquè una dama arribi a una posició final, emmagatzemant indices de captures i salts.
bool Tauler::trobaCadenaDama(const Fitxa estatTauler[NUM_FILES_TAULER][NUM_COLUMNES_TAULER], int filaActualIndex, int columnaActualIndex, int filaDestinacioIndex, int columnaDestinacioIndex, int filesCapturaIndex[], int columnesCapturaIndex[], int& nombreCaptura, int filesSaltIndex[], int columnesSaltIndex[], int& nombreSalt) const
{
    if (filaActualIndex == filaDestinacioIndex && columnaActualIndex == columnaDestinacioIndex)
    {
        return true;
    }

    for (int direccioIndex = 0; direccioIndex < 4; direccioIndex++)
    {
        int filaMitjaIndex;
        int columnaMitjaIndex;

        int filaSaltIndex;
        int columnaSaltIndex;

        if (!potFerSalt(estatTauler, filaActualIndex, columnaActualIndex, direccioIndex, filaMitjaIndex, columnaMitjaIndex, filaSaltIndex, columnaSaltIndex, estatTauler[filaActualIndex][columnaActualIndex]))
        {
            continue;
        }

        filesCapturaIndex[nombreCaptura] = filaMitjaIndex;
        columnesCapturaIndex[nombreCaptura] = columnaMitjaIndex;
        nombreCaptura++;

        filesSaltIndex[nombreSalt] = filaSaltIndex;
        columnesSaltIndex[nombreSalt] = columnaSaltIndex;
        nombreSalt++;

        Fitxa copiaEstat[NUM_FILES_TAULER][NUM_COLUMNES_TAULER];
        clonarEstat(estatTauler, copiaEstat);

        copiaEstat[filaActualIndex][columnaActualIndex] = Fitxa();
        copiaEstat[filaMitjaIndex][columnaMitjaIndex] = Fitxa();
        copiaEstat[filaSaltIndex][columnaSaltIndex] = estatTauler[filaActualIndex][columnaActualIndex];

        if (trobaCadenaDama(copiaEstat, filaSaltIndex, columnaSaltIndex, filaDestinacioIndex, columnaDestinacioIndex, filesCapturaIndex, columnesCapturaIndex, nombreCaptura, filesSaltIndex, columnesSaltIndex, nombreSalt))
        {
            return true;
        }

        nombreCaptura--;
        nombreSalt--;
    }

    return false;
}

//Determina si a la casella intermèdia hi ha una fitxa adversària i a la casella de destí del salt hi ha espai lliure.
bool Tauler::potFerSalt(const Fitxa estatTauler[NUM_FILES_TAULER][NUM_COLUMNES_TAULER], int filaActualIndex, int columnaActualIndex, int indexDireccio, int& filaMitjaIndex, int& columnaMitjaIndex, int& filaSaltIndex, int& columnaSaltIndex, const Fitxa& fitxaOrigen) const
{
    int desplaFilaIndex;
    int desplaColumnaIndex;
    obtenirDesplacamentDiagonal(indexDireccio, desplaFilaIndex, desplaColumnaIndex);

    int sentitFilesIndex = obtenirSentitFiles(fitxaOrigen);

    if (fitxaOrigen.getTipus() == TIPUS_NORMAL && desplaFilaIndex != sentitFilesIndex) { return false; }

    filaMitjaIndex = filaActualIndex + desplaFilaIndex;
    columnaMitjaIndex = columnaActualIndex + desplaColumnaIndex;

    if (fitxaOrigen.getTipus() == TIPUS_DAMA)
    {
        while (posicioEstaDinsTauler(filaMitjaIndex, columnaMitjaIndex) && estatTauler[filaMitjaIndex][columnaMitjaIndex].getTipus() == TIPUS_EMPTY)
        {
            filaMitjaIndex += desplaFilaIndex;
            columnaMitjaIndex += desplaColumnaIndex;
        }
    }

    filaSaltIndex = filaMitjaIndex + desplaFilaIndex;
    columnaSaltIndex = columnaMitjaIndex + desplaColumnaIndex;

    if (!posicioEstaDinsTauler(filaMitjaIndex, columnaMitjaIndex)) { return false; }

    if (!posicioEstaDinsTauler(filaSaltIndex, columnaSaltIndex)) { return false; }

    if (estatTauler[filaMitjaIndex][columnaMitjaIndex].getTipus() == TIPUS_EMPTY) { return false; }

    if (!esRival(fitxaOrigen, estatTauler[filaMitjaIndex][columnaMitjaIndex])) { return false; }

    if (estatTauler[filaSaltIndex][columnaSaltIndex].getTipus() != TIPUS_EMPTY) { return false; }

    return true;
}


// Calcula el nombre màxim de peces que pot capturar una fitxa des d'una posició donada, explorant totes les possibles cadenes de salts consecutius.
int Tauler::maxCapturaDesDe(const Posicio& posicioOrigen) const
{
    Posicio posicionsDestinacio[MAX_MOVIMENTS_POSSIBLES];
    int nombreDestins = 0;
    getPosicionsPossibles(posicioOrigen, nombreDestins, posicionsDestinacio);

    Fitxa taulerAuxiliar[NUM_FILES_TAULER][NUM_COLUMNES_TAULER];
    vectorAArray(taulerAuxiliar);

    int filaOrigen = posicioOrigen.getFilaIndex();
    int columnaOrigen = posicioOrigen.getColumnaIndex();
    int nombreMaximCaptures = 0;

    for (int indexDestinacio = 0; indexDestinacio < nombreDestins; ++indexDestinacio)
    {
        int filaDestinacio = posicionsDestinacio[indexDestinacio].getFilaIndex();
        int columnaDestinacio = posicionsDestinacio[indexDestinacio].getColumnaIndex();

        int capturesActuals = calculaCapturesEntrePosicions(taulerAuxiliar, filaOrigen, columnaOrigen, filaDestinacio, columnaDestinacio);

        if (capturesActuals > nombreMaximCaptures)
        {
            nombreMaximCaptures = capturesActuals;
        }
    }

    return nombreMaximCaptures;
}

