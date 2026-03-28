#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <time.h>
#include "LinkedListApi.h"
#include "wheelTimer.h"

#define TH_JOINABLE 0
#define TH_DETACHED 1

wheel_timer_t *init_wheel_timer(int wheel_size, int clock_tic_interval){

    wheel_timer_t *wheel_timer = (wheel_timer_t *)calloc(1, sizeof(wheel_timer_t) + (sizeof(ll_t *) * wheel_size));
    wheel_timer->current_clock_tic = 0;
    wheel_timer->clock_tic_interval = clock_tic_interval;
    wheel_timer->wheel_size = wheel_size;
    wheel_timer->current_cycle_no = 0;

    for(int i=0; i < wheel_size; i++){
        wheel_timer->slots[i] = intit_singly_ll();
    }

    return wheel_timer;
}

wheel_timer_elem_t *
register_app_event(wheel_timer_t *wt,a
                   app_call_back call_back,
                   void *arg,
                   int arg_size,
                   int time_interval,
                   char is_recurrence){

    wheel_timer_elem_t *wheel_timer_elem = (wheel_timer_elem_t *)calloc(1, sizeof(wheel_timer_elem_t));
    wheel_timer_elem->app_callback = call_back;
    wheel_timer_elem->arg = calloc(1, arg_size);
    memcpy(wheel_timer_elem->arg, arg, arg_size);
    wheel_timer_elem->arg_size = arg_size;
    wheel_timer_elem->timer_interval = time_interval;
    wheel_timer_elem->is_recurrence = is_recurrence;

    
    /* Find the slot where the wt_elem needs to be placed . Insert wheel_timer_elem into the 
       slot's linked list. Calculat r and slot no*/

    return wheel_timer_elem;
}

void start_wheel_timer(wheel_timer_t *wheel_timer){

    if(pthread_create(&wheel_timer->wheel_thread, NULL, wheel_fn, (void *)wheel_timer) != 0) {
        perror("Failed to create wheel timer thread");
        exit(EXIT_FAILURE);
    }
}
