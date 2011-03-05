#ifndef SKIRMISH_RANDOM
#define SKIRMISH_RANDOM

float random_float();
int random_int_max(int);
int random_int();

// should be called at beginning and 
// end of program
void allocate_rng();
void free_rng();

#endif
