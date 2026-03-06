#include "funzioni1.h"

// Inizializza le condizioni iniziali per le stelle
void dati(Star *stella) {
    real x_init[N]  = {3, 3, -1, -3, 2, -2, 2};
    real y_init[N]  = {3, -3, 2, 0, 0, -4, 4};
    real vx_init[N] = {0, 0, 0, 0, 0, 1.75, -1.5};
    real vy_init[N] = {0, 0, 0, -1.25, 1, 0, 0};

    for (int i = 0; i < N; i++) {
        (stella + i)->x = x_init[i];
        (stella + i)->y = y_init[i];
        (stella + i)->vx = vx_init[i];
        (stella + i)->vy = vy_init[i];
        (stella + i)->mass = i + 1; // Masse: 1, 2, …, 7
    }
}

// Esegue un singolo passo di integrazione usando il metodo RK4
void runge_kutta(Star *stella, real dt) {
    real k1x[N], k1y[N], k1vx[N], k1vy[N];
    real k2x[N], k2y[N], k2vx[N], k2vy[N];
    real k3x[N], k3y[N], k3vx[N], k3vy[N];
    real k4x[N], k4y[N], k4vx[N], k4vy[N];
    real ax[N], ay[N];

    // Calcolo le accelerazioni
    auto accelerazione = [&](const Star *s, real *ax, real *ay) {
        for (int i = 0; i < N; i++) {
            ax[i] = 0;
            ay[i] = 0;
            for (int j = 0; j < N; j++) {
                if (i != j) {
                    real dx = s[j].x - s[i].x;
                    real dy = s[j].y - s[i].y;
                    real r = pow(dx * dx + dy * dy, 1.5);  // (dx² + dy²)^(3/2)
                    ax[i] += s[j].mass * dx / r;
                    ay[i] += s[j].mass * dy / r;
                }
            }
        }
    };

    // Calcolo di k1
    accelerazione(stella, ax, ay);
    for (int i = 0; i < N; i++) {
        k1x[i]  = dt * stella[i].vx;
        k1y[i]  = dt * stella[i].vy;
        k1vx[i] = dt * ax[i];
        k1vy[i] = dt * ay[i];
    }

    // k2: stato intermedio
    Star temp[N];
    for (int i = 0; i < N; i++) {
        temp[i].x = stella[i].x + 0.5 * k1x[i];
        temp[i].y = stella[i].y + 0.5 * k1y[i];
        temp[i].vx = stella[i].vx + 0.5 * k1vx[i];
        temp[i].vy = stella[i].vy + 0.5 * k1vy[i];
        temp[i].mass = stella[i].mass;
    }
    accelerazione(temp, ax, ay);
    for (int i = 0; i < N; i++) {
        k2x[i]  = dt * temp[i].vx;
        k2y[i]  = dt * temp[i].vy;
        k2vx[i] = dt * ax[i];
        k2vy[i] = dt * ay[i];
    }

    // k3: stato intermedio
    for (int i = 0; i < N; i++) {
        temp[i].x = stella[i].x + 0.5 * k2x[i];
        temp[i].y = stella[i].y + 0.5 * k2y[i];
        temp[i].vx = stella[i].vx + 0.5 * k2vx[i];
        temp[i].vy = stella[i].vy + 0.5 * k2vy[i];
        temp[i].mass = stella[i].mass;
    }
    accelerazione(temp, ax, ay);
    for (int i = 0; i < N; i++) {
        k3x[i]  = dt * temp[i].vx;
        k3y[i]  = dt * temp[i].vy;
        k3vx[i] = dt * ax[i];
        k3vy[i] = dt * ay[i];
    }

    // k4: stato al tempo t + dt
    for (int i = 0; i < N; i++) {
        temp[i].x = stella[i].x + k3x[i];
        temp[i].y = stella[i].y + k3y[i];
        temp[i].vx = stella[i].vx + k3vx[i];
        temp[i].vy = stella[i].vy + k3vy[i];
        temp[i].mass = stella[i].mass;
    }
    accelerazione(temp, ax, ay);
    for (int i = 0; i < N; i++) {
        k4x[i]  = dt * temp[i].vx;
        k4y[i]  = dt * temp[i].vy;
        k4vx[i] = dt * ax[i];
        k4vy[i] = dt * ay[i];
    }

    // Aggiorna lo stato delle stelle combinando i coefficienti
    for (int i = 0; i < N; i++) {
        stella[i].x  += (k1x[i] + 2*k2x[i] + 2*k3x[i] + k4x[i]) / 6.0;
        stella[i].y  += (k1y[i] + 2*k2y[i] + 2*k3y[i] + k4y[i]) / 6.0;
        stella[i].vx += (k1vx[i] + 2*k2vx[i] + 2*k3vx[i] + k4vx[i]) / 6.0;
        stella[i].vy += (k1vy[i] + 2*k2vy[i] + 2*k3vy[i] + k4vy[i]) / 6.0;
    }
}

// Funzione che simula l'evoluzione del sistema e salva le traiettorie in un file CSV.
// Inoltre, memorizzo i dati in array dinamici per poterli poi analizzar.
void simulazione(Star *stella, const char* filename,
                           real *&traiet_x, real *&traiet_y, real *&tempo_arr, int &nSteps) {
    nSteps = steps; // Il numero di punti per ciascuna traiettoria è "steps"
    //array: dimensione = N * (steps) per le traiettorie, steps per il tempo.
    traiet_x = new real[N * steps];
    traiet_y = new real[N * steps];
    tempo_arr = new real[steps];

    ofstream file(filename);
    if (!file) {
        cerr << "Errore: impossibile aprire " << filename << endl;
        nSteps = 0;
        return;
    }
    file << "Time,Star,X,Y\n";

    for (int step = 0; step < steps; step++) {
        real t = step * dt;
        tempo_arr[step] = t;
        for (int i = 0; i < N; i++) {
            int index = i * steps + step;
            traiet_x[index] = stella[i].x;
            traiet_y[index] = stella[i].y;
            file << t << "," << (i + 1) << "," << stella[i].x << "," << stella[i].y << "\n";
        }
        runge_kutta(stella, dt);
    }

    file.close();
    cout << "Simulazione completata! Dati salvati in " << filename << endl;
}

// Funzione che analizza i dati delle traiettorie per determinare la coppia di stelle
// che raggiunge la distanza minima e il tempo in cui ciò si verifica.

void collisione(const real* traiet_x, const real* traiet_y, const real* tempo_arr,
                int nSteps, int &collisione_stella1, int &collisione_stella2,
                real &collisione_tempo, real &min_distanza) {
    int passo = nSteps;  // Il numero di punti per ciascuna traiettoria

    // Inizializza con la distanza tra la prima coppia (stella 1 e stella 2) al passo 0
    collisione_stella1 = 1;
    collisione_stella2 = 2;
    collisione_tempo = tempo_arr[0];

    real dx = traiet_x[0] - traiet_x[passo];
    real dy = traiet_y[0] - traiet_y[passo];
    min_distanza = sqrt(dx * dx + dy * dy);

    // Scorro tutti i punti e per ciascun punto esamino tutte le coppie di stelle
    for (int j = 0; j < passo; j++) {
        for (int i = 0; i < N; i++) {
            for (int k = i + 1; k < N; k++) {
                int index_i = i * passo + j;
                int index_k = k * passo + j;
                dx = traiet_x[index_i] - traiet_x[index_k];
                dy = traiet_y[index_i] - traiet_y[index_k];
                real dist = sqrt(dx * dx + dy * dy);
                if (dist < min_distanza) {
                    min_distanza = dist;
                    collisione_stella1 = i + 1;
                    collisione_stella2 = k + 1;
                    collisione_tempo = tempo_arr[j];
                }
            }
        }
    }
}
