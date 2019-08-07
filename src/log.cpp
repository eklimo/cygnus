#include "log.h"

Logger::Logger(LogLevel level)
	: level_(level)
{}

void Logger::set_level(LogLevel level)
{
	level_ = level;
}
