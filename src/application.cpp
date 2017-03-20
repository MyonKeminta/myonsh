#include "application.h"
#include <string>
#include <vector>
#include <iostream>
#include "script_interpreter.h"
#include "interactive_source.h"
#include "stream_source.h"
#include "utils.h"
#include "cinclude/cunistd.h"
#include "history.h"
#include "cstdlib"


static std::string historyFile;

static void onExit()
{
	History::save(historyFile);
}

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
//	interactive = true;

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

//	initialize();
	if (interactive)
		Environment::getInstance()->doTrap();

	ScriptInterpreter interpreter;

	if (interactive)
	{
		file = Environment::getInstance()->getHome() + "/.myonshrc";
		std::string command;
		if (!::access(file.c_str(), R_OK))
			command = "source " + file;
		else
			command = "";

		historyFile = Environment::getInstance()->getHome() + "/.myonsh_history";

		History::tryLoad(historyFile);

		atexit(onExit);

		return interpreter.interactive(command);
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


