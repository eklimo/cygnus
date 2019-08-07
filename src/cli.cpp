#include "cli.h"

#include <fstream>

#include "log.h"
#include "compiler.h"
#include "util.h"

namespace CLI
{
	struct Options
	{
		std::vector<std::string_view> inputs;
		bool debug;
	};

	Options parse_options(const std::vector<std::string_view> &args)
	{
		Options options =
		{
			.inputs = {},
			.debug = false
		};

		for(const auto &arg : args)
		{
			if(arg[0] == '-')
			{
				if(arg == "-d" || arg == "--debug")
				{
					options.debug = true;
				}
				else
				{
					Logger::get().warn("Invalid option '", arg, "'");
				}
			}
			else
			{
				options.inputs.push_back(arg);
			}
		}

		return options;
	}

	void execute(const std::vector<std::string_view> &args)
	{
		auto options = parse_options(args);

		if(options.debug)
		{
			Logger::get().set_level(DEBUG);
			Logger::get().info("Enabled debug logging");
		}

		if(options.inputs.empty())
		{
			Logger::get().error("No inputs given");
			std::exit(0);
		}

		for(const auto &input : options.inputs)
		{
			try
			{
				std::string source = Util::read_file(input);
				Logger::get().info("Compiling file '", input, "'");
				Compiler::compile(input, source);
			}
			catch(std::ifstream::failure &e) {}
		}
	}
}
