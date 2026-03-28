#include "wheelTimer.h"

void wrapper_print_hello(void *arg, int arg_size){

    char *st = (char *)arg;
    print_hello(st);
}

int main(int argc, char *argv[]){

    /* create a wheel timer object */
    wheel_timer_t *wheel_timer = init_wheel_timer(WHEEL_SIZE, WHEEL_TIMER_CLOCK_TIC_INTERVAL);
    /* start the wheel timer thread */
    start_wheel_timer(wheel_timer);

    /* Now wheel timer has started running in a separate thread 
       Registert the events to be triggered with whee timer */

    wheel_timer_elem_t *wt_elem = register_app_event(wheel_timer, wrapper_print_hello, "Hello World", strlen("Hello World"), 
                                   5, /* func will be called every 5 sec*/ 
                                   1  /* 1 - indefinitey, 0 - only once */);

    /* stop the main program from getting terminated, else wheel timer thread will be terminated */
    scanf("\n");

    return 0;
}
