#include "funzioni1.h"

int main() {
    // Creo un array statico di stelle e inizializzo i dati
    Star stelle[N];
    dati(stelle);

    // Eseguio la simulazione e salvo i dati in un file CSV,
    // memorizzando anche le traiettorie in array dinamici per l'analisi.
    real *traiettoria_x = nullptr;
    real *traiettoria_y = nullptr;
    real *tempo_arr = nullptr;
    int nSteps = 0;

    simulazione(stelle, "traiettorie_tempo.csv",
                          traiettoria_x, traiettoria_y, tempo_arr, nSteps);
    if (nSteps == 0) {
        delete[] traiettoria_x;
        delete[] traiettoria_y;
        delete[] tempo_arr;
        return 1;
    }

    // Analizza i dati per determinare la coppia di stelle con la distanza minima e il tempo corrispondente
    int stella_collisione1, stella_collisione2;
    real collisione_tempo, min_distanza;
    collisione(traiettoria_x, traiettoria_y, tempo_arr, nSteps,
               stella_collisione1, stella_collisione2, collisione_tempo, min_distanza);

    cout << "Minima distanza: " << min_distanza << endl;
    cout << "Coppia di stelle: Stella " << stella_collisione1
         << " e Stella " << stella_collisione2 << endl;
    cout << "Tempo: " << collisione_tempo << endl;

    // Libera la memoria dinamica allocata
    delete[] traiettoria_x;
    delete[] traiettoria_y;
    delete[] tempo_arr;

    return 0;
}
