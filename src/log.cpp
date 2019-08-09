#include "log.h"

Logger::Logger(LogLevel _level)
	: level(_level)
{}

void Logger::set_level(LogLevel _level)
{
	level = _level;
}
