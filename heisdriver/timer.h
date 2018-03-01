
//Returns seconds since time NULL
double get_wall_time(void);

//Initializes variable timestamp
void timer_init();

//Returns seconds since time NULL + variable time (= 3 sec)
void timer_set(double time);

//Checks if time (3 sec) seconds has passed since timer_set
int timer_check();
