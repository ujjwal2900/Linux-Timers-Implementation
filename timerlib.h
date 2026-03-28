#ifndef __TIMER_WRAP__
#define __TIMER_WRAP__

#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

typedef enum {

    TIMER_INIT,
    TIMER_RUNNING,
    TIMER_CANCELLED,
    TIMER_PAUSED,
    TIMER_RESUME,
    TIMER_DELETED
} TIMER_STATE_T;


typedef struct Timer_ {

    timer_t posix_timer;        /* Posix timer */
    void *user_arg;      /* User defined argument to be passed to callback function */
    unsigned long exp_timer;    /* Expiration timer in milli-seconds */
    unsigned long sec_expo_timer;    /* Second expiration timer in milli-seconds */
    uint32_t threshold;             /* THe number of times to invoke the callback*/
    void (*cb)(struct Timer_ *, void *);  /* Timer callback function */
    bool exponential_backoff;

    /* Place holder value to store 
     * dynamic attributes of timer */
    unsigned long time_remaining;  /* Time left for paused timer for next expiration */  
    uint32_t invocation_counter;    /* How many times the timer has been invoked */
    struct itimerspec ts;           /* specify the exp and sec-exp time intervals */
    unsigned long exp_back_off_timer;    /* Exponential backoff time interval */
    TIMER_STATE_T timer_state;      /* Current state of the timer */

} Timer_t;

/* Returns NULL if timer creation fails, else 
  * returns pointer to the created timer object */

Timer_t *setup_timer(
    /* Timer callback with user data and user size */
    void (*) (Timer_t*, void *),
    /* First expiration timer interval in msec */
    unsigned long,
    /* subsequent expiration timer interval in msec */
    unsigned long,
    /* Max no of expirations, 0 for infinite */
    uint32_t,
    /* argument to timer callback */
    void *,
    /* Is timer Exp back off */
    bool
);


unsigned long timespec_to_millisec(struct timespec *ts);

void timer_fill_itimerspec(struct timespec *ts, unsigned long millisec);


static inline TIMER_STATE_T timer_get_current_state(Timer_t *timer ) {

    if(timer){
        return timer->timer_state;
    }
    return TIMER_DELETED;
}


static inline void timer_set_state(Timer_t *timer, TIMER_STATE_T timer_state) {

    if(timer){
        timer->timer_state = timer_state;
    }
    
}

void resurrect_timer(Timer_t *timer);
void start_timer(Timer_t *timer);   

/* Return the time remaining for the timer  to Fire/Expire */
unsigned long timer_get_time_remaining_in_millisec(Timer_t *timer);

/* Pause the timer */
void  pause_timer(Timer_t *timer);

/* Resume the paused timer */
void resume_timer(Timer_t *timer);

/* Delete the timer */
void delete_timer(Timer_t *timer);

/* Restart the timer */
void restart_timer(Timer_t *timer);

/* Cancel the timer */
void cancel_timer(Timer_t *timer);

/* Reschedule the timer */

#endif /* __TIMER_WRAP__ */
