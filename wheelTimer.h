#ifndef __WHEEL_TIMER_
#define __WHEEL_TIMER_

#include <stdlib.h>
#include <pthread.h>

/* Function pounter - any func of the signature - 
   void *fn (void *,int ) can be invoked by this function pointer */
typedef void (*app_call_back)(void *arg, int sizeof_arg);


typedef struct _wheel_timer_elem_t{

    int timer_interval; // time interval after which the event needs to be triggered next
    int execute_cycle_no; // after how many rotation this event needs to be triggered (r value)
    app_call_back app_callback; // the application function this event needs to execute
    void *arg; // argument to be passed to the application function
    int arg_size; // arg size
    char is_recurrence; // if 1, then this event needs to trigger after every time_interval sec, else only once

}wheel_timer_elem_t;

typedef struct _wheel_timer_t {

    int current_clock_tic; //current slot no which is pointed by the clock tic, it in c by 1 per sec
    int clock_tic_interval;     // timer interval by which the clock tic moves
    int wheel_size; //total no of slots
    int current_cycle_no; // total no of rotations the clock tic has completed
    pthread_t wheel_thread; // thread of wheel timer
    ll_t *slots[0]; // linked list attached to the slots of the wheel timer
}wheel_timer_t;

wheel_timer_elem_t *init_wheel_timer(int wheel_size, int clock_tic_interval);

/* Function to start the wheel timer */
void start_wheel_timer(wheel_timer_t *wheel_timer);

/* Function to be executed by the wheel timer thread */
void *wheel_fn(void *arg);

/* Function to register the app event */
wheel_timer_elem_t *
register_app_event(wheel_timer_t *wt,
                   app_call_back call_back,
                   void *arg,
                   int arg_size,
                   int time_interval,
                   char is_recurrence);



#endif __WHEEL_TIMER_
