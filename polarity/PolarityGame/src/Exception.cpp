#include "Exception.h"

#include <sstream>

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

Exception::Exception(ExceptionCode code, const std::string source, const std::string &desc,
		  const std::string &file, long line)
{
	m_code = code;
	m_source = source;
	m_description = desc;
	m_file = file;
	m_line = line;
}

std::string Exception::getFullDesc() const
{
	std::stringstream desc;
	std::string code;

	switch (m_code)
	{
	case ERR_INTERNAL_ERROR:	code = "ERR_INTERNAL_ERROR";	break;
	case ERR_CANNOT_WRITE_TO_FILE:	code = "ERR_CANNOT_WRITE_TO_FILE";	break;
	case ERR_FILE_NOT_FOUND:	code = "ERR_FILE_NOT_FOUND";	break;
	case ERR_INVALID_PARAM:	code = "ERR_INVALID_PARAM";	break;
	case ERR_NOT_IMPLEMENTED:	code = "ERR_NOT_IMPLEMENTED";	break;
	}

	desc << "Exception " << code << ": " << m_description << " in " << m_source;
	desc << " at " << m_file << " (line " << m_line << ")";

	return desc.str();
}

void Exception::messageBox()
{
#ifdef _WIN32
	MessageBox(NULL, getFullDesc().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#endif
}