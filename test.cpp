#include <iostream>
#include "timerlib.h"
#include <string>

static void user_defined_callback(Timer_t *timer, void *arg) {

    std::cout << "Timer expired! User arg: " << (char *)arg << std::endl;
}

int main(int argc, char **argv){

    std::string name = "Itachii";

    Timer_t *timer = setup_timer(user_defined_callback, 1000, 1000, 0, (void *)name.c_str(), false);
    start_timer(timer);

    std::cout << "1. Pause Timer " << std::endl;
    std::cout << "2. Resume Timer " << std::endl;
    std::cout << "3. Restart Timer " << std::endl;
    std::cout << "4. Reschedule Timer " << std::endl;
    std::cout << "5. Delete Timer " << std::endl;
    std::cout << "6. Cancel Timer " << std::endl;
    std::cout << "7. Get Remaining Time " << std::endl;
    std::cout << "8. Print Timer State " << std::endl;

    int choice = 0;

    while(1){

        std::cin >> choice;

        switch (choice)
        {
            case 1:
                //Pause timer;
                break;
            case 2:
                //Resume timer;
                break;
            case 3:
                //Restart timer;
                break;
            case 4:
                //Reschedule timer;
                break;  
            case 5:
                //Delete timer;
                break;
            case 6:
                //Cancel timer;
                break;
            case 7:
                //Get remaining time;
                break;
            case 8:
                std::cout << "Current Timer State: " << timer_get_current_state(timer) << std::endl;
                break;

        }
        
    }

    pause();

    return 0;
}