#include <signal.h>
#include <time.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <memory.h>
#include <unistd.h>

static void print_current_system_time() {

    time_t t;
    time(&t);   /* Get the current system time */

    /* Print the current system time */
    std::cout << ctime(&t);
}

typedef struct pair_{

    int a;
    int b;
} pair_t;

pair_t pair = { 10, 20};

/* The timer callback function which will be called every 
 * time the timer expires. THe signature of the function is:
 *  void <fun_name> (union sigval )*/

void timer_callback(union sigval arg){

    print_current_system_time();
    
    pair_t *pair = (pair_t *)arg.sival_ptr; /* Extrace the user data structure */

    fprintf(stdout, "pair : [%u %u] \n", pair->a, pair->b);
}

void timer_demo() {

    int ret;
    struct sigevent evp;

    timer_t timer;
    
    memset(&timer, 0, sizeof(timer_t));

    /* evp variable is used to setup timer properties */
    memset(&evp, 0, sizeof(struct sigevent));

    /* Fill the user defined data structure.
     * When timer expires, this will be passed as
     * argument to the timer callback handler */
    evp.sigev_value.sival_ptr = (void *)&pair;

    /* On timer Expiry, we want kernel to launch the 
     * timer handler routine in a separate thread context */
    evp.sigev_notify = SIGEV_THREAD;

    /* Register the timer handler routine. This routine shall be 
     * invoked when timer expires */
    evp.sigev_notify_function = timer_callback;

    /* Create a timer. It is just a timer initialization, Timer is not fired (Alarmed )*/
    ret = timer_create (CLOCK_REALTIME,
                        &evp,
                        &timer);
    
    if(ret < 0) {
        
        fprintf(stderr , "Timer Creation failed, errno = %d\n", errno);
        exit(0);
    }

    /* Let us setup the time intervals */

    struct itimerspec ts;
    memset(&ts, 0, sizeof(ts));

    /* Time to fire for the first timer after 5 seconds 
     * and 0 nano seconds */
    ts.it_value.tv_sec = 5;
    ts.it_value.tv_nsec = 0;

    ts.it_interval.tv_sec = 3;     /*  For periodic timer */
    ts.it_interval.tv_nsec = 0;

    /* Start the timer */
    ret = timer_settime (timer,
                        0,
                        &ts,
                        NULL);
    
    if(ret < 0){

        fprintf(stderr, "Timer Start failed, errno = %d\n", errno);
        exit(0);
    }
    else{
        print_current_system_time();
        fprintf(stdout, "Timer Alarmed successfully \n");
    }

}




int main(int argc, char **argv)
{
    timer_demo();
    pause();

    return 0;
}