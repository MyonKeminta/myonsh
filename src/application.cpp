#include "application.h"
#include <string>
#include <vector>
#include <iostream>
#include "script_interpreter.h"
#include "interactive_source.h"
#include "stream_source.h"
#include "utils.h"

Application *Application::instance = nullptr;

Application *Application::getInstance()
{
	if (instance)
		return instance;
	return instance = new Application;
}

int Application::run(const std::vector<std::string> &args)
{
	Environment::initWithArgs(args);

	interactive = !Environment::getInstance()->isInputRedirected();

	std::string file;

	bool hasFile = false;

	for (unsigned i =1; i != args.size(); ++i)
	{
		const auto &arg = args[i];

		// Check if arg is an option
		if (!arg.empty() && arg[0] == '-')
		{
			// But no option is provided.
			// Report a invalid-arg error
			Log::LogError("Invalid argument: " + arg);
			return 127;
		}
		else
		{
			// If arg is not a option, regard it as a file
			file = arg;
			hasFile = true;
			interactive = false;
		}
	}

	initialize();

	ScriptInterpreter interpreter;

	if (interactive)
	{
		file = Environment::getInstance()->getHome() + "/.myonshrc";
		interpreter.executeString("source " + file);
		return interpreter.interactive();
	}
	else
	{
		if (hasFile)
			return interpreter.executeString("source " + file);
		else
			return interpreter.executeString("source /dev/stdin");
	}


	return 0;
}


