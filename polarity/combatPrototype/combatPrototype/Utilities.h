#ifndef _Utilities_h_
#define _Utilities_h_

#include <string>
#include <sstream>

inline std::string intToString( int aInt ) {
	std::stringstream ss;
	ss << aInt;
	return ss.str();
}

#endif // _Utilities_h_