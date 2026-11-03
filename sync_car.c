#include "sync_car.h"

void station_init(struct station *station){
    pthread_mutex_init(&station->lock, NULL);

    pthread_cond_init(&station->car_arrived, NULL);
    pthread_cond_init(&station->all_boarded, NULL);

    station->waiting_passengers  = 0;
    station->free_seats          = 0;
    station->boarding_passengers = 0;
}
void station_load_car(struct station *station, int count){

}
void station_wait_for_car(struct station *station){
    pthread_mutex_lock(&station->lock);

    station->waiting_passengers++;

    while (station->free_seats == 0)
        pthread_cond_wait(&station->car_arrived, &station->lock);

    station->waiting_passengers--;
    station->free_seats--;
    station->boarding_passengers++;

    pthread_mutex_unlock(&station->lock);
}
void station_on_board(struct station *station){
    pthread_mutex_lock(&station->lock);

    station->boarding_passengers--;

    if (station->boarding_passengers == 0)
        pthread_cond_signal(&station->all_boarded);

    pthread_mutex_unlock(&station->lock);
}