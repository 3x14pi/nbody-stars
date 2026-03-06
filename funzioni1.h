#ifndef FUNZIONI1_H
#define FUNZIONI1_H

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

typedef double real;

// Costanti globali
const int N = 7;          // Numero di stelle
const real T = 3.0;       // Tempo massimo della simulazione
const real dt = 0.01;     // Passo temporale
const int steps = int(T / dt) + 1;  // Numero di punti per ciascuna traiettoria

// Struttura che rappresenta una stella
struct Star {
    real x, y;    // Coordinate della posizione
    real vx, vy;  // Componenti della velocità
    real mass;    // Massa
};

// Dichiarazioni delle funzioni

// Inizializza le condizioni iniziali delle stelle
void dati(Star *stella);

// Esegue un singolo passo di integrazione con il metodo di Runge-Kutta a 4 passi
void runge_kutta(Star *stella, real dt);

// Simula l'evoluzione del sistema nel tempo, salvando le traiettorie in un file CSV
// E contemporaneamente salva i dati in array dinamici per l'analisi.
void simulazione(Star *stella, const char* filename,
                           real *&traiet_x, real *&traiet_y, real *&tempo_arr, int &nSteps);

// Analizza i dati delle traiettorie per determinare la coppia di stelle con la distanza minima
// e il tempo in cui ciò si verifica.
void collisione(const real* traiet_x, const real* traiet_y, const real* tempo_arr,
                int nSteps, int &collisione_stella1, int &collisione_stella2,
                real &collisione_tempo, real &min_distanza);

#endif
