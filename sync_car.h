#ifndef SYNC_CAR_H
#define SYNC_CAR_H

#include <pthread.h>

struct station {
    pthread_mutex_t passengers_lock;
    pthread_mutex_t boarding_lock;

    int waiting_passengers;
    int free_seats;
    int boarding_passengers;

    pthread_cond_t car_arrived;
    pthread_cond_t seat_taken;
    pthread_cond_t all_boarded;
};

void station_init(struct station *station);
void station_load_car(struct station *station, int count);
void station_wait_for_car(struct station *station);
void station_on_board(struct station *station);

#endif