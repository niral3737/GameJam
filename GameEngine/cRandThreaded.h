#ifndef _Utilities_HG_
#define _Utilities_HG_

#include <Windows.h>	// for the critical section

// Inspired by: https://stackoverflow.com/questions/686353/c-random-float-number-generation
template <class T>
T getRandInRange_RT( T min, T max )
{
	double value = 
		min + static_cast <double> (rand()) 
		/ ( static_cast <double> (RAND_MAX/(static_cast<double>(max-min) )));
	return static_cast<T>(value);
}

// Inspired by: https://stackoverflow.com/questions/686353/c-random-float-number-generation
template <class T>
T getRandInRange_RT(T min, T max, T randValueFrom0to1)
{
	double value = min + randValueFrom0to1 * static_cast<double>( max - min );
	return static_cast<T>( value );
}


// Either lock the rand each time (which might be OK)
// Create a ring buffer of rand calls, with a function to get the "next" one, 
//	always writing to the values in advance.

//CRITICAL_SECTION CS_rand
//
//int getRand()
//{
//	EnterCriticalSection( CS_rand );
//	int theRand = rand();
//	LeaveCriticalSection( CS_rand );
//	return theRand;
//}

class cRandThreaded
{
public:
	cRandThreaded();
	~cRandThreaded();
	double getNextRandDouble(void);

	static const int SIZEOFBUFFER = 100;
	double randDoubles[SIZEOFBUFFER];
	int lastReadIndex;	// = 0;
	CRITICAL_SECTION CS_bufferIndexLock;
private:
	void m_LoadBufferWithRandoms(void);
};


#endif
