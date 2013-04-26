#ifndef RANDOM_H
#define RANDOM_H
/*******************************************************************************************
 * random
 * this random generatro garantee below:
 * 
 * once you have create a random with specifed seed
 * the random queue produced by random between reset will always be the same
 *
 *******************************************************************************************/
 
typdef struct random Random;

//create a random generator with specified seed
Random *random_create(int seed);

//reset the generator, then the below sequence will be the same
void random_reset(Random *random);

//get an int from the random
int  random_next(Random *random);

//destroy a random generator
void random_destroy(Random *random);
#endif
