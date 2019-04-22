#ifndef _JSON_UTILS_HG_
#define _JSON_UTILS_HG_

#include <string>

#include "json.hpp"

using json = nlohmann::json;

class cJsonUtils
{
public:
	static json getJsonInstance();
private:
	cJsonUtils();
	static json j;
};

#endif // !_JSON_UTILS_HG_



