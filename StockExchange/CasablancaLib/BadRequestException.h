
#include <iostream>

class BadRequestException : public std::exception
{
public:
	BadRequestException(std::string message);
	~BadRequestException();

private:

};

BadRequestException::BadRequestException(std::string message) : exception(message.c_str())
{
}

BadRequestException::~BadRequestException()
{
}