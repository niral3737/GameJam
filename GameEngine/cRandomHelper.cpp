#include "cRandomHelper.h"
#include <cstdlib>
#include <iostream>



cRandomHelper::cRandomHelper()
{}


cRandomHelper::~cRandomHelper()
{}

unsigned int cRandomHelper::generateRandomIntInRange(unsigned int start, unsigned int end)
{
	return start + (std::rand() % static_cast<int>(end - start + 1));
}

float cRandomHelper::generateRandomfloatInRange(float start, float end)
{
	return start + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (end - start)));
}
