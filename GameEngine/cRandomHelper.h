#ifndef _RANDOM_HELPER_HG_
#define _RANDOM_HELPER_HG_

class cRandomHelper
{
public:
	cRandomHelper();
	~cRandomHelper();

	//this generates the number including the range
	static unsigned int generateRandomIntInRange(unsigned int start, unsigned int end);
	static float generateRandomfloatInRange(float start, float end);
};

#endif // !_RANDOM_HELPER_HG_



