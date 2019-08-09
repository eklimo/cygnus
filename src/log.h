#pragma once

#include <iostream>

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

	template<typename... Args>
	inline void debug(Args &&... args) const { log(DEBUG, std::forward<Args>(args)...); }
	template<typename... Args>
	inline void info(Args &&... args) const { log(INFO, std::forward<Args>(args)...); }
	template<typename... Args>
	inline void warn(Args &&... args) const { log(WARNING, std::forward<Args>(args)...); }
	template<typename... Args>
	inline void error(Args &&... args) const { log(ERROR, std::forward<Args>(args)...); }


private:
	LogLevel level;

	constexpr const char *prefix(LogLevel level) const
	{
		switch(level)
		{
			case DEBUG:
				return "\033[1;32m[D]\033[0m ";
			case INFO:
				return "\033[1;37m[I]\033[0m ";
			case WARNING:
				return "\033[1;33m[W]\033[0m ";
			case ERROR:
				return "\033[1;31m[E]\033[0m ";
			default:
				return "";
		}
	}

	template<typename... Args>
	void log(LogLevel _level, Args &&... args) const
	{
		if(level <= _level)
		{
			std::cout << prefix(_level);
			// ((std::cout << args << " "), ...) << std::endl;
			(std::cout << ... << args) << std::endl;
		}
	}
};
