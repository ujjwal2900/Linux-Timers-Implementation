#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <time.h>
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
register_app_event(wheel_timer_t *wt,
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

    /* Calculate the slot and execute_cycle_no */
    int total_tics = time_interval;
    int future_tic = wt->current_clock_tic + total_tics;
    int slot = future_tic % wt->wheel_size;
    int execute_cycle_no = future_tic / wt->wheel_size;
    wheel_timer_elem->execute_cycle_no = execute_cycle_no;

    /* Insert into the slot's linked list */
    singly_ll_node_t *node = singly_ll_init_node(wheel_timer_elem);
    singly_ll_add_node(wt->slots[slot], node);

    return wheel_timer_elem;
}

void start_wheel_timer(wheel_timer_t *wheel_timer){

    if(pthread_create(&wheel_timer->wheel_thread, NULL, wheel_fn, (void *)wheel_timer) != 0) {
        perror("Failed to create wheel timer thread");
        exit(EXIT_FAILURE);
    }
}

void  reset_wheel_timer(wheel_timer_t *wt)
{
    wt->current_clock_tic = 0;
    wt->current_cycle_no = 0;
}

void *wheel_fn(void *arg){

    wheel_timer_t *wheel_timer = (wheel_timer_t *)arg;

    while(1){

        sleep(wheel_timer->clock_tic_interval);
        wheel_timer->current_clock_tic = (wheel_timer->current_clock_tic + 1) % wheel_timer->wheel_size;

        if(wheel_timer->current_clock_tic == 0){
            wheel_timer->current_cycle_no++;
        }

        /* Check the linked list attached to the current slot and execute the events whose execute_cycle_no is equal to current_cycle_no */
        ll_t *slot_ll = wheel_timer->slots[wheel_timer->current_clock_tic];
        singly_ll_node_t *node = slot_ll->head;
        singly_ll_node_t *next_node;

        while(node){
            next_node = node->next;
            wheel_timer_elem_t *elem = (wheel_timer_elem_t *)node->data;
            if(elem->execute_cycle_no == wheel_timer->current_cycle_no){
                elem->app_callback(elem->arg, elem->arg_size);
                if(elem->is_recurrence){
                    /* Re-schedule the event */
                    int total_tics = elem->timer_interval;
                    int future_tic = wheel_timer->current_clock_tic + total_tics;
                    int new_slot = future_tic % wheel_timer->wheel_size;
                    int new_execute_cycle_no = future_tic / wheel_timer->wheel_size;
                    elem->execute_cycle_no = new_execute_cycle_no;
                    /* Remove from current slot */
                    singly_ll_remove_node(slot_ll, node);
                    /* Create new node and add to new slot */
                    singly_ll_node_t *new_node = singly_ll_init_node(elem);
                    singly_ll_add_node(wheel_timer->slots[new_slot], new_node);
                } else {
                    /* Remove and free the event */
                    singly_ll_remove_node(slot_ll, node);
                    free(elem->arg);
                    free(elem);
                }
            }
            node = next_node;
        }
    }
}
