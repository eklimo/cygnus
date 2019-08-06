#pragma once

#include <string>

enum LogLevel
{
	DEBUG, INFO, WARNING, ERROR
};

class Logger
{
public:
	explicit Logger(LogLevel level);
	static Logger &get()
	{
		static Logger logger(INFO);
		return logger;
	}

	void set_level(LogLevel level);
	inline void debug(const std::string &message) const { log(DEBUG, message); }
	inline void info(const std::string &message) const { log(INFO, message); }
	inline void warn(const std::string &message) const { log(WARNING, message); }
	inline void error(const std::string &message) const { log(ERROR, message); }

private:
	LogLevel level_;
	void log(LogLevel level, const std::string &message) const;
};
