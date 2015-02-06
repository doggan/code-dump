#ifndef _Utilities_h_
#define _Utilities_h_

#include "Common.h"

#include <string>
#include <sstream>

// TODO: Let's use Boost lexical cast library:
//http://www.gamedev.net/community/forums/topic.asp?topic_id=312790
//http://www.boost.org/doc/libs/1_35_0/libs/conversion/lexical_cast.htm

template <typename T>
inline std::string toStr(T val)
{
	std::stringstream ss;
	ss << val;
	return ss.str();
}

inline int strToInt(const std::string &str)
{
	int val;
	std::istringstream ss(str);
	ss >> val;

	if (ss.fail())
		EXCEPTION(Exception::ERR_INVALID_PARAM, "strToInt()", "Cannot convert '" + str + "' to an int");

	return val;
}

inline float strToFloat(const std::string &str)
{
	float val;
	std::istringstream ss(str);
	ss >> val;

	if (ss.fail())
		EXCEPTION(Exception::ERR_INVALID_PARAM, "strToFloat()", "Cannot convert '" + str + "' to a float");

	return val;
}

inline void switchToForwardSlashes(std::string &str)
{
	for (uint i = 0; i < str.size(); ++i)
	{
		if (str[i] == '\\')
			str[i] = '/';
	}
}

inline float rand01()
{
	return (float)rand() / (float)(RAND_MAX - 1);
}

inline int round(float val)
{
	if (val >= 0)
		return (int)(val + .5f);
	else
		return (int)(val - .5f);
}

#endif // _Utilities_h_