#include "states.h"
#include "elev.h"
#include "queue.h"
#include "timer.h"


typedef enum{
	INIT,IDLE,MOVING,STOP,STOPBETWEENFLOORS
} state;




static int current_floor;
static int dir;
static int prev_dir;
static int button;
state currentstate = INIT;

void states_init(){
		queue_clear();
		elev_set_motor_direction(DIRN_UP);
		currentstate = INIT;
		while (elev_get_floor_sensor_signal() ==-1) {

		}

	}



	void states_clear_all_lights(){
	        for(int f = 0; f < N_FLOOR; f++){
	            for(button = 0; button < N_BUTTON; button++){
								if ((!(button == 0 && f == N_FLOOR - 1)) && (!(button == 1 && f == 0))){
									elev_set_button_lamp(button,f,0);
								}
	            }
	        }
	}

void states_clear_lights_at_floor(int floor){
	for(button = 0; button < N_BUTTON; button++){
		if ((!(button == 0 && floor == N_FLOOR - 1)) && (!(button == 1 && floor == 0))){
                	elev_set_button_lamp(button,floor,0);
				}
		}
}


void states_elev_arrived_at_floor (int floor){
	current_floor = floor;
	switch (currentstate){
	case INIT:
		elev_set_floor_indicator(current_floor);
		elev_set_motor_direction(DIRN_STOP);
		currentstate = IDLE;
		break;
	case MOVING:
		elev_set_floor_indicator(current_floor);
			if (queue_order_at_floor (current_floor, dir)){
				elev_set_motor_direction(DIRN_STOP);
				elev_set_door_open_lamp(1);
				queue_delete(current_floor);
				states_clear_lights_at_floor(current_floor);
				timer_set(3);
				currentstate = STOP;
			}
			break;
		case IDLE:
		case STOP:
		case STOPBETWEENFLOORS:
		default:
			break;
	}
}

void states_stop_button_pressed(){
	elev_set_motor_direction(DIRN_STOP);
	states_clear_all_lights();
	elev_set_stop_lamp(1);
	queue_clear();

	if (elev_get_floor_sensor_signal() != -1){
		elev_set_door_open_lamp(1);
		timer_set(3);
		currentstate=STOP;
		}
	else{
			elev_set_door_open_lamp(0);
			currentstate=STOPBETWEENFLOORS;
		}

}


void states_stop_button_released(){
	switch (currentstate){
		case STOPBETWEENFLOORS:
		case STOP:
			elev_set_stop_lamp(0);
			if (elev_get_floor_sensor_signal() == -1){
			currentstate = STOPBETWEENFLOORS;
}
			if (elev_get_floor_sensor_signal() !=-1){
				elev_set_door_open_lamp(1);
				timer_set(3);
				currentstate = IDLE;
			}
			break;
		case INIT:
		case MOVING:
		case IDLE:
		default:
			break;
	}
}


void states_get_next_order(){

	if ((currentstate!=STOP)&&(currentstate!=STOPBETWEENFLOORS)&&(currentstate!=MOVING)){
		elev_set_door_open_lamp(0);
		elev_set_stop_lamp(0);
		dir = queue_get_direction(prev_dir,current_floor);
		elev_set_motor_direction(dir);

		if(dir!=DIRN_STOP){
				currentstate = MOVING;
			}
		else {
				currentstate = IDLE;
			}
}
}

void states_new_order_pressed(button, floor){
	switch (currentstate) {
		case IDLE:
			if(floor != current_floor){
				queue_update(button, floor);
				elev_set_button_lamp(button, floor, 1);
				dir = queue_get_direction(prev_dir, current_floor);
				elev_set_motor_direction(dir);
				prev_dir=dir;
				currentstate = MOVING;
			}
			else{
				elev_set_door_open_lamp(1);
				timer_set(3);
			}
			break;
		case MOVING:
			queue_update(button,floor);
			elev_set_button_lamp(button, floor, 1);
			break;
		case STOPBETWEENFLOORS:
			queue_update(button, floor);

			 if (floor==current_floor){
				 elev_set_motor_direction(-(prev_dir));
			 }
			 else {
				 dir = queue_get_direction(prev_dir, current_floor);
				 elev_set_motor_direction(dir);
			 }
			 currentstate = MOVING;
			 break;
		case INIT:
		default:
			break;
	}
}
