#include <stdio.h>


#define N_FLOOR 4
#define N_BUTTON 3

//used if stop-button is pressed. All entries in queue-matrix == 0
void queue_clear();

//deletes orders in current floor (up/down/command)
void queue_delete(int current_floor);

//new order added to queue-matrix
void queue_update(int button, int floor);

int queue_order_at_floor (int floor, int dir);

int queue_orders_below(int floor);

int queue_orders_above(int floor);

int queue_get_direction(int prev_dir, int floor);
