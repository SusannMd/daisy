
#include <stdio.h>
#include <sys/time.h>
#include "timer.h"



double get_wall_time(void){
	struct timeval time;
	gettimeofday(&time, NULL);
	return (double)time.tv_sec + (double)time.tv_usec *.000001;
}

static double timestamp;


void timer_init() {
	timestamp = 0;
}

void timer_set(double time) {
	timestamp = get_wall_time() + time;
}

int timer_check() {
	return get_wall_time() - timestamp > 0;
}
