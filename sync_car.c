#include "sync_car.h"

void station_init(struct station *station){
    pthread_mutex_init(&station->passengers_lock, NULL);
    pthread_mutex_init(&station->boarding_lock, NULL);

    pthread_cond_init(&station->car_arrived, NULL);
    pthread_cond_init(&station->seat_taken, NULL);
    pthread_cond_init(&station->all_boarded, NULL);

    station->waiting_passengers  = 0;
    station->free_seats          = 0;
    station->boarding_passengers = 0;
}
void station_load_car(struct station *station, int count){

}
void station_wait_for_car(struct station *station){

}
void station_on_board(struct station *station){

}