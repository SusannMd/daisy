#include "queue.h"
#include "elev.h"

static int queue_matrix[N_BUTTON][N_FLOOR];
int queue_orders_below(int floor);
int queue_orders_above(int floor);

void queue_clear(){
    for (int i=0; i<N_BUTTON; i++){
        for(int j=0; j<N_FLOOR; j++){
            if (queue_matrix[j][i] != -1){
                queue_matrix[j][i] = 0;
            }
        }
    }
}

//SPM, slette både opp og ned på en gang?
void queue_delete(int current_floor){
    queue_matrix[current_floor][0] = 0;
    queue_matrix[current_floor][1] = 0;
    queue_matrix[current_floor][2] = 0;
    }


void queue_update(int button, int floor){
    queue_matrix[floor][button] = 1;
}

// int queue_order_at_floor (int floor, int dir){
//   for (int i=0; i<N_FLOOR;i++){
//     if (queue_matrix[i][3]){
//       return 1;
//       }
//     }
//   if (dir==1) {
//     queue_matrix[floor][0] = 1;
//     return 1;
//   }
//   if (dir==0) {
//     queue_matrix[floor][1] = 1;
//     return 1;
//   }
// return 0;
// }
int queue_order_at_floor(int floor,int dir){
    if (queue_matrix[floor][BUTTON_COMMAND] == 1){
		return 1;
    }
    if (dir == DIRN_UP){
        if (queue_matrix[floor][BUTTON_CALL_UP] == 1){
            return 1;
		}
        if (queue_orders_above(floor))
            return 0;
    }
    if (dir == DIRN_DOWN){
        if (queue_matrix[floor][BUTTON_CALL_DOWN] == 1){
            return 1;
		}
        if (queue_orders_below(floor)){
            return 0;
		}
    }

    return 1;
}


int queue_orders_below(int floor){
    for (int f = (floor-1); f > -1; f--){
		    for (int button = 0; button < N_BUTTON; button++){
			       if (queue_matrix[f][button]){
				           return 1;
			       }
		     }
	  }
	return 0;
}

int queue_orders_above(int floor){
    for (int f = (floor+1); f < N_FLOOR; f++){
		    for (int button = 0; button < N_BUTTON; button++){
			       if (queue_matrix[f][button]){
				           return 1;
			       }
		     }
	  }
	return 0;
}


int queue_get_direction(int prev_dir, int floor){
	if (prev_dir == DIRN_UP){
		if (queue_orders_above(floor)){
		    return DIRN_UP;
		}
	}
	if (prev_dir == DIRN_DOWN){
		if (queue_orders_below(floor)){
			return DIRN_DOWN;
		}
	}

	if (queue_orders_above(floor)){
		return DIRN_UP;
		}
	if (queue_orders_below(floor)){

		    return DIRN_DOWN;
		    }

	return DIRN_STOP;
}
