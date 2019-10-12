#pragma once

#include <iostream>

enum class LogLevel
{
	Debug, Info, Warning, Error
};

class Logger
{
public:
	explicit Logger(LogLevel level);
	static Logger &get()
	{
		static Logger logger(LogLevel::Info);
		return logger;
	}

	void set_level(LogLevel level);

	template<typename... Args>
	inline void debug(Args &&... args) const
	{
		log(LogLevel::Debug, std::forward<Args>(args)...);
	}
	template<typename... Args>
	inline void info(Args &&... args) const
	{
		log(LogLevel::Info, std::forward<Args>(args)...);
	}
	template<typename... Args>
	inline void warn(Args &&... args) const
	{
		log(LogLevel::Warning, std::forward<Args>(args)...);
	}
	template<typename... Args>
	inline void error(Args &&... args) const
	{
		log(LogLevel::Error, std::forward<Args>(args)...);
	}

private:
	LogLevel level;

	constexpr const char *prefix(LogLevel level) const
	{
		switch(level)
		{
			case LogLevel::Debug:
				return "";
			case LogLevel::Info:
				return "";
			case LogLevel::Warning:
				return "\033[33;1mWarning: \033[0m";
			case LogLevel::Error:
				return "\033[31;1mError: \033[0m";
			default:
				return "";
		}
	}

	template<typename... Args>
	void log(LogLevel level, Args &&... args) const
	{
		if(this->level <= level)
		{
			std::cout << prefix(level);
			// ((std::cout << args << " "), ...) << std::endl;
			(std::cout << ... << args) << "\033[0m" << std::endl;
		}
	}
};
