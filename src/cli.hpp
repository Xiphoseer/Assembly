#pragma once

#define CLI_PATH "cli"

/*! \file cli.hpp
 *  \brief CLI-Tools
 *
 *  Utilities for command line use.
 */

//! command-line-interface tools
namespace cli
{
	typedef int (*main_t) (int, char**);

	typedef struct
	{
		const char* text;
		main_t func;
		const char* help;
	}
	opt_t;

	int call(const char* cli, const opt_t* options, const char* cmd, int argc, char** argv);
	int main(const char* cli, const opt_t* options, const char* cmd, int argc, char** argv);
    int help(const char* cli, const opt_t* options, const char* desc);
}
