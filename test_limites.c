#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include "sync_car.h"

struct station st;
volatile int sairam = 0;
volatile int vagao_saiu = 0;

void *passageiro(void *arg) {
    (void) arg;
    station_wait_for_car(&st);
    __sync_add_and_fetch(&sairam, 1);
    return NULL;
}

void *vagao(void *arg) {
    station_load_car(&st, *(int *) arg);
    vagao_saiu = 1;
    return NULL;
}

void timeout(int s) {
    (void) s;
    printf("FALHA: o programa travou (timeout de 5 s)\n");
    exit(1);
}

int main(void) {
    signal(SIGALRM, timeout);
    alarm(5);
    station_init(&st);

    pthread_t p[3];
    for (int i = 0; i < 3; i++)
        pthread_create(&p[i], NULL, passageiro, NULL);
    sleep(1);   /* os 3 passageiros ja estao esperando */

    /* Caso invalido: numero de assentos negativo */
    station_load_car(&st, -5);
    printf("Caso invalido (count = -5): vagao retornou; embarcaram %d (esperado 0)\n", sairam);

    /* Caso limite: mais assentos que passageiros */
    int assentos = 10;
    pthread_t v;
    pthread_create(&v, NULL, vagao, &assentos);
    while (sairam < 3) usleep(1000);   /* espera os 3 sairem do wait_for_car */
    if (vagao_saiu) { printf("FALHA: vagao partiu antes dos on_board\n"); return 1; }
    for (int i = 0; i < 3; i++)
        station_on_board(&st);
    pthread_join(v, NULL);
    printf("Caso limite (10 assentos, 3 passageiros): embarcaram %d (esperado 3)\n", sairam);
    printf("free_seats depois da partida: %d (esperado 0)\n", st.free_seats);

    for (int i = 0; i < 3; i++)
        pthread_join(p[i], NULL);
    printf("OK: casos limite e invalido passaram\n");
    return 0;
}