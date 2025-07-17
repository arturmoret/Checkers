#include "joc.hpp"
#include "GraphicManager.h"
#include <iostream>
#include <fstream>

using namespace std;

//Carrega el tauler inicial i prepara la cua de moviments
Joc::Joc()
{
    const string FITXER_TAULER = "data/Games/tauler_inicial.txt";
    const string FITXER_MOVS = "data/Games/moviments.txt";

    if (!inicialitza(MODE_JOC_NORMAL, FITXER_TAULER, FITXER_MOVS))
    {
        cout << "No s'ha pogut carregar " << FITXER_TAULER << '\n';
    }
}

//Intercanvia el torn entre blanc i negre
void Joc::canviaTorn()
{
    if (m_torn == COLOR_BLANC)
    {
        m_torn = COLOR_NEGRE;
    }

    else
    {
        m_torn = COLOR_BLANC;
    }
}

//Retorna true si les coordenades de finestra (x,y) són dins la zona del tauler
bool Joc::posicioDinsTauler(int x, int y) const
{
    int margeEsquerra = POS_X_TAULER + CASELLA_INICIAL_X;
    int margeSuperior = POS_Y_TAULER + CASELLA_INICIAL_Y;

    int ampladaTauler = NUM_COLS_TAULER * AMPLADA_CASELLA;
    int alcadaTauler = NUM_FILES_TAULER * ALCADA_CASELLA;

    int margeDret = margeEsquerra + ampladaTauler;
    int margeInferior = margeSuperior + alcadaTauler;

    bool dinsAmplada = (x >= margeEsquerra) && (x < margeDret);
    bool dinsAlcada = (y >= margeSuperior) && (y < margeInferior);

    return dinsAmplada && dinsAlcada;
}


//Converteix coordenades de finestra (x,y) a fila i columna del tauler
void Joc::coordAPosTauler(int x, int y, int& fila, int& col) const
{
    int margeEsquerra = POS_X_TAULER + CASELLA_INICIAL_X;
    int margeSuperior = POS_Y_TAULER + CASELLA_INICIAL_Y;

    int posicioRelativaX = x - margeEsquerra;
    int posicioRelativaY = y - margeSuperior;

    col = posicioRelativaX / AMPLADA_CASELLA;
    fila = posicioRelativaY / ALCADA_CASELLA;
}


// Inicialitza l’estat del joc i carrega la cua de moviments si és mode replay
bool Joc::inicialitza(ModeJoc mode, const string& fitxerTauler, const string& fitxerMoviments)
{

    m_mode = mode;
    m_acabada = false;
    m_torn = COLOR_BLANC;
    m_nomFitxerMoviments = fitxerMoviments;

    m_tauler.inicialitza(fitxerTauler);

    if (m_mode == MODE_JOC_REPLAY)
    {
        if (!m_cuaMoviments.loadFromFile(fitxerMoviments))
        {
            cout << "No s'ha pogut carregar " << fitxerMoviments << '\n';

            return false;
        }
    }

    return true;
}

bool Joc::existeixMoviment(ColorFitxa color) const
{
    for (int filaTauler = 0; filaTauler < NUM_FILES_TAULER; filaTauler++)
    {
        for (int columnaTauler = 0; columnaTauler < NUM_COLUMNES_TAULER; columnaTauler++)
        {
            const Fitxa& fitxaActual = m_tauler.getFitxa(filaTauler, columnaTauler);

            if (fitxaActual.getTipus() == TIPUS_EMPTY)
            {
                continue;
            }

            if (fitxaActual.getColor() != color)
            {
                continue;
            }

            string nomCasella = string(1, 'a' + columnaTauler) + to_string(NUM_FILES_TAULER - filaTauler);
            Posicio posicioOrigen = Posicio(nomCasella);

            Posicio posicionsDestins[MAX_MOVIMENTS_POSSIBLES];
            int totalDestins = 0;

            m_tauler.getPosicionsPossibles(posicioOrigen, totalDestins, posicionsDestins);

            if (totalDestins > 0)
            {
                return true;
            }
        }
    }

    return false;
}

//Dibuixa el fons i el tauler al començament 
void Joc::pintaFonsITauler() const
{
    GraphicManager* gestorGrafics = GraphicManager::getInstance();

    gestorGrafics->drawSprite(GRAFIC_FONS, 0, 0);
    gestorGrafics->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER);
}

//Ressalta la casella d’origen seleccionat i les destinacions possibles
void Joc::dibuixaRessalts() const
{
    if (!m_origenSeleccionat)
    {
        return;
    }

    int columnaOrigen = m_origen.getColumnaIndex();
    int filaOrigen = m_origen.getFilaIndex();

    int posicioXOrigen = POS_X_TAULER + CASELLA_INICIAL_X + columnaOrigen * AMPLADA_CASELLA;
    int posicioYOrigen = POS_Y_TAULER + CASELLA_INICIAL_Y + filaOrigen * ALCADA_CASELLA;

    GraphicManager::getInstance()->drawSprite(GRAFIC_POSICIO_VALIDA, posicioXOrigen, posicioYOrigen);

    for (int indexDestinacio = 0; indexDestinacio < m_nDestins; indexDestinacio++)
    {
        int columnaDestinacio = m_destins[indexDestinacio].getColumnaIndex();
        int filaDestinacio = m_destins[indexDestinacio].getFilaIndex();

        int posicioX = POS_X_TAULER + CASELLA_INICIAL_X + columnaDestinacio * AMPLADA_CASELLA;
        int posicioY = POS_Y_TAULER + CASELLA_INICIAL_Y + filaDestinacio * ALCADA_CASELLA;

        GraphicManager::getInstance()->drawSprite(GRAFIC_POSICIO_VALIDA, posicioX, posicioY);
    }
}

// Dibuixa totes les fitxes del tauler segons el seu color i tipus
void Joc::dibuixaFitxes() const
{
    for (int fila = 0; fila < NUM_FILES_TAULER; fila++)
    {
        for (int columna = 0; columna < NUM_COLUMNES_TAULER; columna++)
        {
            const Fitxa& fitxaActual = m_tauler.getFitxa(fila, columna);

            if (fitxaActual.getTipus() == TIPUS_EMPTY)
            {
                continue;
            }

            IMAGE_NAME graficaFitxa;

            if (fitxaActual.getColor() == COLOR_BLANC)
            {
                if (fitxaActual.getTipus() == TIPUS_DAMA)
                {
                    graficaFitxa = GRAFIC_DAMA_BLANCA;
                }
                else
                {
                    graficaFitxa = GRAFIC_FITXA_BLANCA;
                }
            }
            else
            {
                if (fitxaActual.getTipus() == TIPUS_DAMA)
                {
                    graficaFitxa = GRAFIC_DAMA_NEGRA;
                }
                else
                {
                    graficaFitxa = GRAFIC_FITXA_NEGRA;
                }
            }

            int posicioX = POS_X_TAULER + CASELLA_INICIAL_X + columna * AMPLADA_CASELLA;
            int posicioY = POS_Y_TAULER + CASELLA_INICIAL_Y + fila * ALCADA_CASELLA;

            GraphicManager::getInstance()->drawSprite(graficaFitxa, posicioX, posicioY);
        }
    }
}


//Mostra informació de mode, torn, guanyador i missatge de fi de replay
void Joc::dibuixaHUD()
{
    string textMode = "";

    if (m_mode == MODE_JOC_NORMAL)
    {
        textMode = "Mode: Normal";
    }
    else
    {
        if (m_mode == MODE_JOC_REPLAY)
        {
            textMode = "Mode: Replay";
        }
        else
        {
            textMode = "Mode: Fi del Replay";
        }
    }

    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, POS_X_TAULER, POS_Y_TAULER + NUM_FILES_TAULER * ALCADA_CASELLA + 150, 1.0f, textMode);

    if (!m_acabada)
    {
        string textTorn = "";

        if (m_torn == COLOR_BLANC)
        {
            textTorn = "Torn: Blanques";
        }
        else
        {
            textTorn = "Torn: Negres";
        }

        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, POS_X_TAULER, POS_Y_TAULER + NUM_FILES_TAULER * ALCADA_CASELLA + 110, 1.0f, textTorn);
    }

    if (m_acabada)
    {
        string textGuanyador = "";

        if (m_guanyador == COLOR_BLANC)
        {
            textGuanyador = "Guanyen les blanques!";
        }
        else
        {
            textGuanyador = "Guanyen les negres!";
        }

        GraphicManager::getInstance()->drawFont(FONT_GREEN_30, POS_X_TAULER, POS_Y_TAULER + 720, 1.2f, textGuanyador);
    }

    if (m_missatgeReplay)
    {
        GraphicManager::getInstance()->drawFont(FONT_RED_30, POS_X_TAULER, POS_Y_TAULER + NUM_FILES_TAULER * ALCADA_CASELLA + 230, 0.75f, "No hi ha mes moviments a reproduir");
    }
}


//Métode per gestionar la tecla S per desar i R per carregar o iniciar replay
void Joc::teclatDesaOCarrega()
{
    bool teclaDesarMin = Keyboard_GetKeyTrg('s');
    bool teclaDesarMaj = Keyboard_GetKeyTrg('S');
    bool teclaReproduirMin = Keyboard_GetKeyTrg('r');
    bool teclaReproduirMaj = Keyboard_GetKeyTrg('R');

    bool esPartidaNormal = (m_mode == MODE_JOC_NORMAL);

    if (teclaDesarMin)
    {
        if (esPartidaNormal)
        {
            m_cuaMoviments.saveToFile(m_nomFitxerMoviments);
        }
    }

    if (teclaDesarMaj)
    {
        if (esPartidaNormal)
        {
            m_cuaMoviments.saveToFile(m_nomFitxerMoviments);
        }
    }

    if (teclaReproduirMin || teclaReproduirMaj)
    {
        string camiFitxerTauler = "data/Games/tauler_inicial.txt";

        m_tauler.inicialitza(camiFitxerTauler);

        bool carregaCorrecta = m_cuaMoviments.loadFromFile(m_nomFitxerMoviments);

        if (carregaCorrecta)
        {
            m_mode = MODE_JOC_REPLAY;
            m_replayIndex = 0;
            m_acabada = false;
        }
        else
        {
            //missatge per fer depuració
            cout << "No s’ha pogut obrir " << m_nomFitxerMoviments << "\n";
        }
    }
}


//Processa un clic en mode normal, selecciona peça o mou fitxa
void Joc::processaClicModeNormal(int posicioX, int posicioY, bool botoAlliberat)
{
    if (!botoAlliberat)
    {
        return;
    }

    if (!posicioDinsTauler(posicioX, posicioY))
    {
        return;
    }

    int filaTauler, columnaTauler;

    coordAPosTauler(posicioX, posicioY, filaTauler, columnaTauler);

    char lletraColumna = 'a' + columnaTauler;
    int numeroFila = NUM_FILES_TAULER - filaTauler;
    Posicio posicioClicada(string(1, lletraColumna) + to_string(numeroFila));

    if (!m_origenSeleccionat)
    {
        const Fitxa& fitxaSeleccionada = m_tauler.getFitxa(filaTauler, columnaTauler);

        bool esBuida = (fitxaSeleccionada.getTipus() == TIPUS_EMPTY);
        bool esDelTornActual = (fitxaSeleccionada.getColor() == m_torn);

        if (!esBuida && esDelTornActual)
        {
            int filaObligatoria, columnaObligatoria;
            int capturesMaximesGlobals = m_tauler.calcularMaximaCapturaGlobal(
                m_torn, filaObligatoria, columnaObligatoria);

            int capturesDesDeFitxa = m_tauler.maxCapturaDesDe(posicioClicada);

            bool noHiHaCapturaObligatoria = (capturesMaximesGlobals == 0);
            bool estaAutoritzada = (capturesDesDeFitxa == capturesMaximesGlobals);

            if (noHiHaCapturaObligatoria || estaAutoritzada)
            {
                m_origen = posicioClicada;
                m_origenSeleccionat = true;
                m_tauler.getPosicionsPossibles(m_origen, m_nDestins, m_destins);
            }
        }
    }
    else
    {
        bool movimentValid = m_tauler.mouFitxa(m_origen, posicioClicada);

        if (movimentValid)
        {
            Moviment movimentNou;
            movimentNou.afegirPosicio(m_origen);
            movimentNou.afegirPosicio(posicioClicada);
            m_cuaMoviments.pushBack(movimentNou);

            canviaTorn();

            if (!existeixMoviment(m_torn))
            {
                m_acabada = true;

                if (m_torn == COLOR_BLANC)
                {
                    m_guanyador = COLOR_NEGRE;
                }
                else
                {
                    m_guanyador = COLOR_BLANC;
                }
            }
        }

        m_origenSeleccionat = false;
        m_nDestins = 0;
    }
}


//Processa un clic en mode replay, avança o acaba el replay
void Joc::processaClicModeReplay(bool botoAlliberat)
{
    static bool botoBloquejat = false;

    if (!botoAlliberat || botoBloquejat)
    {
        botoBloquejat = botoAlliberat;
        return;
    }

    int totalMoviments = m_cuaMoviments.size();

    if (m_replayIndex < totalMoviments)
    {
        Moviment movimentActual = m_cuaMoviments[m_replayIndex];
        m_replayIndex = m_replayIndex + 1;

        Posicio posicioOrigen = movimentActual.getPosicio(0);
        Posicio posicioDesti = movimentActual.getPosicio(1);
        m_tauler.mouFitxa(posicioOrigen, posicioDesti);
    }
    else
    {
        m_mode = MODE_JOC_NONE;
        m_missatgeReplay = true;
    }

    botoBloquejat = true;
}

// Aquest métode es el bucle principal, processa entrades i dibuixa cada fotograma
bool Joc::actualitza(int posicioRatoliX, int posicioRatoliY, bool botoRatoliPremut)
{
    pintaFonsITauler();
    teclatDesaOCarrega();

    static bool botoAnteriorPremut = false;
    bool botoAlliberat = (!botoRatoliPremut && botoAnteriorPremut);

    if (m_mode == MODE_JOC_NORMAL && !m_acabada)
    {
        processaClicModeNormal(posicioRatoliX, posicioRatoliY, botoAlliberat);
    }

    else
    {
        if (m_mode == MODE_JOC_REPLAY)
        {
            processaClicModeReplay(botoAlliberat);
        }
    }

    botoAnteriorPremut = botoRatoliPremut;

    dibuixaRessalts();
    dibuixaFitxes();
    dibuixaHUD();

    if (m_acabada)
    {
        finalitza();
    }

    return true;
}

//Finalitza la partida i guarda el estat final del tauler.
void Joc::finalitza()
{
    if (m_mode == MODE_JOC_NORMAL)
    {
        m_cuaMoviments.saveToFile(m_nomFitxerMoviments);
    }

    m_cuaMoviments.clear();

    m_origenSeleccionat = false;

    m_missatgeReplay = false;

    m_replayIndex = 0;
}