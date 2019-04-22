#include "cJsonUtils.h"
#include <fstream>

json cJsonUtils::j = 0;

json cJsonUtils::getJsonInstance()
{
	if (j == 0)
	{
		std::ifstream ifs("settings.json");
		j = json::parse(ifs);
		ifs.close();
	}
	return j;
}
