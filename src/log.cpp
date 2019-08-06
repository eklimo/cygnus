#include "log.h"

#include <iostream>

Logger::Logger(LogLevel level)
	: level_(level)
{}

constexpr const char *prefix(LogLevel level) 
{
	switch(level)
	{
		case DEBUG:
			return "\033[1;32m[debug]\033[0m ";
		case INFO:
			return "\033[1;37m[info]\033[0m ";
		case WARNING:
			return "\033[1;33m[warning]\033[0m ";
		case ERROR:
			return "\033[1;31m[error]\033[0m ";
	}
	return "";
}

void Logger::set_level(LogLevel level)
{
	level_ = level;
}

void Logger::log(LogLevel level, const std::string &message) const
{
	if(level_ <= level)
	{
		std::cout << prefix(level) << message << std::endl;
	}
}
