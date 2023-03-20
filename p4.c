#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

#define TAM 512000

typedef struct {
    int vector[TAM];
    int ultimo;
} monticulo;

void inicializar_semilla() {
    srand(time(NULL));
}

void intercambiar(monticulo *M, int i, int j) {
    int aux;
    aux = M->vector[i];
    M->vector[i] = M->vector[j];
    M->vector[j] = aux;
}

void hundir(monticulo *M, int i) {
    int hijoIzq, hijoDer, j;
    hijoIzq = 2 * i + 1;
    hijoDer = 2 * i + 2;
    j = i;
    if (hijoIzq <= M->ultimo && M->vector[hijoIzq] > M->vector[j]) {
        j = hijoIzq;
    }
    if (hijoDer <= M->ultimo && M->vector[hijoDer] > M->vector[j]) {
        j = hijoDer;
    }
    if (j != i) {
        intercambiar(M, i, j);
        hundir(M, j);
    }
}

void crear_monticulo(const int v[], int n, monticulo *M) {
    int i;
    for (i = 0; i < n; i++) {
        M->vector[i] = v[i];
    }
    M->ultimo = n - 1;
    for (i = (n / 2); i >= 0; i -= 1) {
        hundir(M, i);
    }
}

int eliminar_mayor(monticulo *m) {
    int x;
    if (m->ultimo == -1) printf("Error: Monticulo vacío\n");
    else {
        x = m->vector[0];
        m->vector[0] = m->vector[m->ultimo];
        m->ultimo = m->ultimo - 1;
        if (m->ultimo >= 0) hundir(m, 0);
        return x;
    }
    return -1;
}


void ascendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = i + 1;
    }
}

void descendente(int v[], int n) {
    int i, j;
    j = 1;
    for (i = n - 1; i >= 0; i--) {
        v[i] = j;
        j++;
    }
}

void aleatorio(int v[], int n) {
    int i, m = 2 * n + 1;
    for (i = 0; i < n; i++)
        v[i] = (rand() % m) - n;
}


void print_monticulo(monticulo *M) {
    int cnt = 1,i;
    for (i = 0; i <= M->ultimo; i++) {
        if (ceil(log2(cnt)) == floor(log2(cnt))) {
            printf("\n");
        }
        if (i == 0) {
            printf("%d(raíz) ", M->vector[i]);
        } else {
            printf("%d(hijo de %d) ", M->vector[i], M->vector[(i - 1) / 2]);
        }
        cnt++;
    }
    if (M->ultimo == -1) {
        printf("El montículo está vacío\n");
    }
}

void test() {
    monticulo M;
    int n=8, i, x, v[n];
    aleatorio(v, n);
    crear_monticulo(v, n, &M);
    printf("\n~~~~TEST DE MONTÍCULOS~~~~\n");
    printf("\nEL MONTÍCULO INICIAL ES:\n");
    print_monticulo(&M);
    printf("\n");
    for (i = 0; i < n; i++) {
        x = eliminar_mayor(&M);
        printf("\nELIMINAMOS EL MAYOR (%d)\n", x);
        print_monticulo(&M);
        printf("\n");
    }
}

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return ((double) t.tv_usec + (double) t.tv_sec * 1000000.0);
}

void ordenacion_por_monticulos(int v[], int n) {
    monticulo M;
    int i;
    crear_monticulo(v, n, &M);
    for (i = (n - 1); i >= 0; i--) {
        v[i] = eliminar_mayor(&M);
    }
}


void tiempos() {
    int n, i, v[TAM];
    double t1, t2, x, y, z, t;
    monticulo M;
    printf("\nCREACIÓN DE MONTÍCULOS:\n\n");
    for (n = 1000; n <= TAM; n = n * 2) {
        ascendente(v, n);
        t1 = microsegundos();
        crear_monticulo(v, n, &M);
        t2 = microsegundos();
        t = t2 - t1;
        if (t < 500) {
            t1 = microsegundos();
            for (i = 0; i < 1000; i++) {
                crear_monticulo(v, n, &M);
            }
            t2 = microsegundos();
            t = (t2 - t1) / 1000;
            printf("(*)");
        } else {
            printf("   ");
        }
        x = t / pow(n, 0.8);
        y = t / n;
        z = t / pow(n, 1.2);
        printf("%12d%15.6lf%15.6lf%15.6lf%15.6lf\n", n, t, x, y, z);
    }
    printf("\n");
}

void tiempos_ascendente() {
    int n, i, v[TAM];
    double t1, t2, x, y, z, t;
    printf("\nASCENDENTE:\n\n");
    for (n = 1000; n <= TAM; n = n * 2) {
        ascendente(v, n);
        t1 = microsegundos();
        ordenacion_por_monticulos(v, n);
        t2 = microsegundos();
        t = t2 - t1;
        if (t < 500) {
            t1 = microsegundos();
            for (i = 0; i < 1000; i++) {
                ascendente(v, n);
                ordenacion_por_monticulos(v, n);
            }
            t2 = microsegundos();
            t = (t2 - t1);
            t1 = microsegundos();
            for (i = 0; i < 1000; i++) {
                ascendente(v, n);
            }
            t2 = microsegundos();
            t = (t - (t2 - t1)) / 1000;
            printf("(*)");
        } else {

            printf("   ");
        }
        x = t / pow(n * log(n), 0.9);
        y = t / (n * log(n));
        z = t / pow(n * log(n), 1.1);
        printf("%12d%15.6lf%15.6lf%15.6lf%15.6lf\n", n, t, x, y, z);
    }
    printf("\n");
}

void tiempos_descendente() {
    int n, i, v[TAM];
    double t1, t2, x, y, z, t;
    printf("\nDESCENDENTE\n\n");
    for (n = 1000; n <= TAM; n = n * 2) {
        descendente(v, n);
        t1 = microsegundos();
        ordenacion_por_monticulos(v, n);
        t2 = microsegundos();
        t = t2 - t1;
        if (t < 500) {
            t1 = microsegundos();
            for (i = 0; i < 1000; i++) {
                descendente(v, n);
                ordenacion_por_monticulos(v, n);
            }
            t2 = microsegundos();
            t = (t2 - t1);
            t1 = microsegundos();
            for (i = 0; i < 1000; i++) {
                descendente(v, n);
            }
            t2 = microsegundos();
            t = (t - (t2 - t1)) / 1000;
            printf("(*)");
        } else {

            printf("   ");
        }
        x = t / pow(n * log(n), 0.9);
        y = t / (n * log(n));
        z = t / pow(n * log(n), 1.1);
        printf("%12d%15.6lf%15.6lf%15.6lf%15.6lf\n", n, t, x, y, z);
    }
    printf("\n");
}

void tiempos_aleatorio() {
    int n, i, v[TAM];
    double t1, t2, x, y, z, t;
    printf("\nALEATORIO\n\n");
    for (n = 1000; n <= TAM; n = n * 2) {
        aleatorio(v, n);
        t1 = microsegundos();
        ordenacion_por_monticulos(v, n);
        t2 = microsegundos();
        t = t2 - t1;
        if (t < 500) {
            t1 = microsegundos();
            for (i = 0; i < 1000; i++) {
                aleatorio(v, n);
                ordenacion_por_monticulos(v, n);
            }
            t2 = microsegundos();
            t = (t2 - t1);
            t1 = microsegundos();
            for (i = 0; i < 1000; i++) {
                aleatorio(v, n);
            }
            t2 = microsegundos();
            t = (t - (t2 - t1)) / 1000;
            printf("(*)");
        } else {

            printf("   ");
        }
        x = t / pow(n * log(n), 0.9);
        y = t / pow((n * log(n)), 1.04);
        z = t / pow(n * log(n), 1.1);
        printf("%12d%15.6lf%15.6lf%15.6lf%15.6lf\n", n, t, x, y, z);
    }
    printf("\n");
}

void test_ordenacion() {
    int n = 10, v[n], i;
    printf("\nCOMPROBAMOS SI UN VECTOR SE ORDENA BIEN:\n");
    aleatorio(v, n);
    printf("EL VECTOR DESORDENADO ES:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    ordenacion_por_monticulos(v, n);
    printf("\nEL VECTOR ORDENADO ES:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
}


int main() {
    inicializar_semilla();
    test();
    printf("\n");
    tiempos();
    printf("\n");
    test_ordenacion();
    printf("\n");
    tiempos_ascendente();
    tiempos_descendente();
    tiempos_aleatorio();
}
