#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H


#include <exception>
#include <stdexcept>
#include <string>


class UnexpectedEofException : std::logic_error
{
public:
	explicit UnexpectedEofException(const std::string &what)
		: std::logic_error(what) {}
	explicit UnexpectedEofException(const char *what)
		: std::logic_error(what) {}
};

class SyntaxError : std::logic_error
{
public:
	explicit SyntaxError(const std::string &what)
		: std::logic_error(what) {}
	explicit SyntaxError(const char *what)
		: std::logic_error(what) {}
};


#endif //EXCEPTIONS_H
