#ifndef OPTO_H
#define OPTO_H

enum Optocoupler_side{
	OP_RIGHT = 3,
	OP_LEFT = 2,
	};

int opto_turn (int, int);

#endif