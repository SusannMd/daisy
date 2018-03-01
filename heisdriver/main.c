#include "elev.h"
#include <stdio.h>
#include "queue.h"
#include "timer.h"
#include "channels.h"
#include "states.h"



int last_floor_visited = 0;

int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }


    states_init();


    while(1){

       int floor = elev_get_floor_sensor_signal();
       if(floor != last_floor_visited  &&  floor != -1){
           states_elev_arrived_at_floor(floor);
       }
       last_floor_visited = floor;

           if(elev_get_stop_signal()){
             states_stop_button_pressed();

           }
           else {
             states_stop_button_released();
           }

       for (int buttonfloor = 0; buttonfloor < N_FLOOR; buttonfloor++){
         for (int buttontype = 0; buttontype < N_BUTTON; buttontype++){

           if ((!(buttontype == 0 && buttonfloor == N_FLOOR - 1)) && (!(buttontype == 1 && buttonfloor == 0))) {
             int test = elev_get_button_signal(buttontype,buttonfloor);
              if (test){
               states_new_order_pressed(buttontype, buttonfloor);
                   if ((buttonfloor == last_floor_visited)&&(elev_get_floor_sensor_signal()!=-1)){
                     states_elev_arrived_at_floor(last_floor_visited);
              }
            }
          }
        }
     }


   if (timer_check()){
     states_get_next_order();
   }
 }

return 0;
}
