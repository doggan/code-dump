#ifndef _Exception_h_
#define _Exception_h_

#include <exception>
#include <string>

class Exception : public std::exception
{
public:
	enum ExceptionCode
	{
		ERR_INTERNAL_ERROR,
		ERR_CANNOT_WRITE_TO_FILE,
		ERR_FILE_NOT_FOUND,
		ERR_INVALID_PARAM,
		ERR_NOT_IMPLEMENTED,
		ERR_INCORRECT_USAGE
	};

	Exception(ExceptionCode code, const std::string source, const std::string &desc,
			  const std::string &file, long line);
	virtual ~Exception() throw() {}

	void messageBox();

	std::string getFullDesc() const;

	const ExceptionCode getCode() { return m_code; }
	const std::string &getSource() { return m_source; }
	const std::string &getFile() { return m_file; }
	long getLine() { return m_line; }
	const std::string &getDesc() { return m_description; }

	const char *what() const throw() { return getFullDesc().c_str(); }

private:
	ExceptionCode m_code;
	long m_line;
	std::string m_file;
	std::string m_source;
	std::string m_description;
};

#define EXCEPTION(code, source, desc) throw Exception(code, source, desc, __FILE__, __LINE__)

#endif // _Exception_h_