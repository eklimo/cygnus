#include "cli.h"

#include <fstream>

#include "log.h"
#include "util/util.h"
#include "util/error.h"
#include "compiler.h"

namespace CLI
{
	struct Options
	{
		std::vector<std::string_view> inputs;
		bool debug;
		bool help;
	};

	void print_help()
	{
		std::cout <<
		          R"(Usage: cygnus [options] inputs...
Options:
  -h, --help: Print this help message
  -d, --debug: Enable debug logging messages)"
		          << std::endl;
	}

	Options parse_options(const std::vector<std::string_view> &args)
	{
		Options options =
		{
			.inputs = {},
			.debug = false,
			.help = false
		};

		for(const auto &arg : args)
		{
			if(arg[0] == '-')
			{
				if(arg == "-d" || arg == "--debug")
				{
					options.debug = true;
				}
				else if(arg == "-h" || arg == "--help")
				{
					options.help = true;
				}
				else
				{
					Logger::get().warn("invalid option '", arg, "'");
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

		// process options

		if(options.help)
		{
			print_help();
			std::exit(0);
		}

		if(options.debug)
		{
			Logger::get().set_level(LogLevel::Debug);
			Logger::get().info("Enabled debug logging");
		}

		if(options.inputs.empty())
		{
			Logger::get().error("no inputs given");
			Logger::get().info("Run 'cygnus --help' for usage");
			std::exit(0);
		}

		// compile inputs
		for(const auto &input : options.inputs)
		{
			auto dot_index = input.find_last_of(".");
			std::string_view ext = input.substr(dot_index + 1);
			if(dot_index == std::string_view::npos || ext == "")
			{
				Logger::get().error("invalid file format for '", input, "'");
				continue;
			}
			else if(ext != "cy")
			{
				Logger::get().error("invalid file format '", ext, "' for '", input, "'");
				continue;
			}

			try
			{
				std::string source = Util::read_file(input);
				Logger::get().debug("Compiling file '", input, "'");
				try
				{
					Compiler::compile(input, source);
				}
				catch(Util::Error &e)
				{
					e.print(input, source);
					Logger::get().error("terminating compilation for file '", input, "'");
				}
			}
			catch(std::ifstream::failure &e)
			{
				Logger::get().error("unable to open file '", input, "'");
			}

		}
	}
}
