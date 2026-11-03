#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/resource.h>
#include "sync_car.h"

struct station st;

void *passageiro(void *arg) {
    (void) arg;
    station_wait_for_car(&st);
    return NULL;
}

int main(void) {
    station_init(&st);
    pthread_t t[10];
    for (int i = 0; i < 10; i++)
        pthread_create(&t[i], NULL, passageiro, NULL);

    sleep(3);

    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    double cpu = ru.ru_utime.tv_sec + ru.ru_utime.tv_usec / 1e6 + ru.ru_stime.tv_sec + ru.ru_stime.tv_usec / 1e6;
    printf("Tempo de CPU em 3 s de espera: %.4f s\n", cpu);
    printf(cpu < 0.1 ? "OK: sem busy-wait\n" : "FALHA: threads girando\n");
    return 0;
}