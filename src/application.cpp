#include "application.h"
#include <string>
#include <vector>
#include <iostream>
#include "script_interpreter.h"
#include "interactive_source.h"
#include "stream_source.h"

Application *Application::instance = nullptr;

Application *Application::getInstance()
{
	if (instance)
		return instance;
	return instance = new Application;
}

int Application::run(const std::vector<std::string> &args)
{
	interactive = true;

	for (unsigned i =1; i != args.size(); ++i)
	{
		const auto &arg = args[i];

		// Check if arg is an option
		if (!arg.empty() && arg[0] == '-')
		{
			// But no option is provided.
			// Report a invalid-arg error
			return 127;
		}
		else
		{
			// If arg is not a option, regard it as a file
			interactive = false;
		}
	}

	initialize();



	return 0;
}


