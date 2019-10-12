#include "log.h"

Logger::Logger(LogLevel level)
	: level(level)
{}

void Logger::set_level(LogLevel level)
{
	this->level = level;
}
