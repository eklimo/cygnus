#include "cli.h"

#include "log.h"

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
					Logger::get().warn("Invalid option '", arg.data(), "'");
				}
			}
			else
			{
				options.inputs.push_back(arg);
			}
		}

		return options;
	}

	int execute(const std::vector<std::string_view> &args)
	{
		auto options = parse_options(args);

		if(options.inputs.empty())
		{
			Logger::get().error("No inputs given");
			std::exit(0);
		}

		if(options.debug)
		{
			Logger::get().set_level(DEBUG);
			Logger::get().info("Enabled debug logging");
		}

		for(const auto &input : options.inputs)
		{
			Logger::get().debug("Input file: '", input, "'");
		}

		return 0;
	}
}
