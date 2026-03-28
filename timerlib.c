#include "timerlib.h"
#include <assert.h>


unsigned long timespec_to_millisec(struct timespec *ts) {

    unsigned long millisec = 0;

    if (ts) {

        millisec = (ts->tv_sec * 1000) + (ts->tv_nsec / 1000000);
    }

    return millisec;
}

void timer_fill_itimerspec(struct timespec *ts, unsigned long millisec) {

    if (ts) {

        ts->tv_sec = millisec / 1000;
        ts->tv_nsec = (millisec % 1000) * 1000000;
    }
}

static void timer_callback_wrapper(union sigval arg){

    Timer_t *timer = (Timer_t *)(arg.sival_ptr);


    if(timer->timer_state == TIMER_RESUME){

        if(timer->sec_expo_timer != 0){
            timer->timer_state = TIMER_RUNNING;
        }
    }

    /* invoke the user provided callback fn */
    (timer->cb)(timer, timer->user_arg);
}



Timer_t *setup_timer(
    void (*timer_cb) (Timer_t*, void *),
    unsigned long exp_timer,
    unsigned long sec_exp_timer,
    uint32_t threshold,
    void *user_arg,
    bool exponential_backoff
) {

    Timer_t *timer = (Timer_t *)calloc(1, sizeof(Timer_t));    
    memset(&timer->posix_timer, 0, sizeof(timer_t));

    timer->cb = timer_cb;
    timer->exp_timer = exp_timer;
    timer->sec_expo_timer = sec_exp_timer;
    timer->threshold = threshold;
    timer->user_arg = user_arg;
    timer_set_state(timer, TIMER_INIT);
    timer->exponential_backoff = exponential_backoff;   

    /* Sanity check */
    assert(timer->cb);

    struct sigevent evp;
    memset(&evp, 0, sizeof(struct sigevent));

    evp.sigev_value.sival_ptr = (void *)timer;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = timer_callback_wrapper;

    int rc = timer_create (CLOCK_REALTIME, &evp, &timer->posix_timer);

    assert(rc >= 0);

    timer_fill_itimerspec(&timer->ts.it_value, timer->exp_timer);
    timer_fill_itimerspec(&timer->ts.it_interval, timer->sec_expo_timer);

    timer->exp_back_off_timer = 0;

    return timer;
}

void resurrect_timer(Timer_t *timer){

    int rc;
    /* timer->ts value should not be 0, otherwise timer will not start */
    rc = timer_settime(timer->posix_timer, 0, &timer->ts, NULL);
    assert(rc >= 0);
}

void start_timer(Timer_t *timer) {

    resurrect_timer(timer);
    timer_set_state(timer, TIMER_RUNNING);
}

unsigned long timer_get_time_remaining_in_millisec(Timer_t *timer) {

    struct itimerspec remaining_time;
    memset(&remaining_time, 0, sizeof(struct itimerspec));

    timer_gettime(timer->posix_timer, &remaining_time);

    return timespec_to_millisec(&remaining_time.it_value);
}

void pause_timer(Timer_t *timer) {

    timer->time_remaining = timer_get_time_remaining_in_millisec(timer);

    timer_fill_itimerspec(&timer->ts.it_value, 0);
    timer_fill_itimerspec(&timer->ts.it_interval, 0);

    resurrect_timer(timer);

    timer_set_state(timer, TIMER_PAUSED);
}

void resume_timer(Timer_t *timer) {

    if( timer_get_current_state(timer) != TIMER_PAUSED){
        printf("Timer is not in paused state, cannot resume \n");
        assert(0);
    }

    timer_fill_itimerspec(&timer->ts.it_value, timer->time_remaining);
    timer_fill_itimerspec(&timer->ts.it_interval, timer->sec_expo_timer);
    timer->time_remaining = 0;

    resurrect_timer(timer);
}

void delete_timer(Timer_t *timer){

    timer_delete(timer->posix_timer);
    timer_set_state(timer, TIMER_DELETED);
    free(timer);
}

void restart_timer(Timer_t *timer){

    timer_fill_itimerspec(&timer->ts.it_value, timer->exp_timer);
    timer_fill_itimerspec(&timer->ts.it_interval, timer->sec_expo_timer);

    resurrect_timer(timer);
}   

void cancel_timer(Timer_t *timer){

    timer_fill_itimerspec(&timer->ts.it_value, 0);
    timer_fill_itimerspec(&timer->ts.it_interval, 0);

    timer_set_state(timer, TIMER_CANCELLED);
}