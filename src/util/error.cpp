#include "error.h"

std::string Util::CompilerError::what() throw()
{
	return message.str();
}
