//#include "Utilities.h"
#include "cRandThreaded.h"
#include <cstdlib>	// RAND_MAX
#include <Windows.h>	// Critical Sections

//// Inspired by: https://stackoverflow.com/questions/686353/c-random-float-number-generation
//
//double getRandInRange( double min, double max )
//{
//
//	double value = min + static_cast <double> (rand()) / ( static_cast <double> (RAND_MAX/(max-min)));
//	return value;
//}



cRandThreaded::cRandThreaded()
{
	InitializeCriticalSection( &(this->CS_bufferIndexLock) );

	// Load initial list of random numbmers
	this->m_LoadBufferWithRandoms();

	return;
}

cRandThreaded::~cRandThreaded()
{
	DeleteCriticalSection( &(this->CS_bufferIndexLock ) );

	return;
}


double cRandThreaded::getNextRandDouble(void)
{
	EnterCriticalSection( &(this->CS_bufferIndexLock ) );

	this->lastReadIndex++;

	if ( this->lastReadIndex >= cRandThreaded::SIZEOFBUFFER )
	{
		this->m_LoadBufferWithRandoms();
		this->lastReadIndex = 0;
	}

	double theRandomNumber = this->randDoubles[this->lastReadIndex];

	LeaveCriticalSection( &(this->CS_bufferIndexLock ) );

	return theRandomNumber;
};

void cRandThreaded::m_LoadBufferWithRandoms(void)
{
	for ( int index = 0; index != cRandThreaded::SIZEOFBUFFER; index++ )
	{
		this->randDoubles[index] = getRandInRange_RT<double>(0.0, 1.0);
	}
	this->lastReadIndex = 0;

	return;
}

