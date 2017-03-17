#include "script_interpreter.h"
#include <string>
#include <stack>
#include <fstream>
#include "utils.h"
#include "script_source.h"
#include "interactive_source.h"
#include "stream_source.h"
#include "context.h"
#include "preprocessor.h"
#include <cerrno>
#include <regex>


ScriptInterpreter::ScriptInterpreter() {}

int ScriptInterpreter::executeString(const std::string &cmd)
{
	StringList preprocessedCmd = Preprocessor(context).process(cmd);
	run(preprocessedCmd);
	return context.getLastExitCode();
}

int ScriptInterpreter::interactive()
{
//	sourceStack.push(InteractiveSource(context));
	InteractiveSource source(context);
	runFromScriptSource(source, true);
	return context.getLastExitCode();
}

void ScriptInterpreter::sourceAnotherFile(const StringList &cmd)
{
	if (cmd.size() <= 1)
	{
		Log::LogError("source: needs argument.\n"
					  "source: usage: source file [args...]");
		context.setLastExitCode(2);
		return;
	}
	std::ifstream file(cmd[1]);
	if (!file)
	{
		Log::LogError("source: failed opening file " + cmd[1]);
	}

	StreamSource source(file);
	runFromScriptSource(source);
}

void ScriptInterpreter::alias(const StringList &args)
{
	if (args.size() <= 1)
	{
		auto dict = context.getAliasDictionary();
		for (const auto &pair : dict)
		{
			std::cout << "alias " << pair.first << "='"
					  << pair.second <<'\'' << std::endl;
		}
	}
	else if (args[1].find('=') == std::string::npos)
	{
		auto it = context.getAliasDictionary().find(args[1]);
		if (it == context.getAliasDictionary().end())
		{
			Log::LogError("alias: " + args[1] +": not found");
			context.setLastExitCode(1);
			return;
		}
		else
		{
			std::cout << "alias " << it->first << "='"
					  << it->second << '\'' << std::endl;
		}
	}
	else
	{
		unsigned split = 0;
		while (args[1][split] != '=')
			++split;
		std::string name(args[1].begin(), args[1].begin() + split);
		if (!std::regex_match(name, std::regex("[a-zA-Z_][a-zA-Z0-9_]*")))
		{
			Log::LogError("alias: " + name + ": invalid alias");
			context.setLastExitCode(1);
			return;
		}
		else
		{
			context.getAliasDictionary()[name] =
					std::string(args[1].begin() + split + 1, args[1].end());
		}
	}
	context.setLastExitCode(0);
}

void ScriptInterpreter::cd(const StringList &args)
{
	std::string target;
	if (args.size() <= 1)
	{
		target = Environment::getInstance()->getHome();
	}
	else if (args[1] == "-")
	{
		if (context.hasVar("OLDPWD"))
		{
			target = context.getVar("OLDPWD");
		}
		else
		{
			Log::LogError("cd: OLDPWD not set");
			context.setLastExitCode(1);
			return;
		}
	}
	else
	{
		target = args[1];
	}

	if (Environment::getInstance()->chdir(target))
	{
		context.setLastExitCode(errno);
		Log::LogErrorByErrno("cd: ");
	}
	else
		context.setLastExitCode(0);
}


void ScriptInterpreter::run(const StringList &cmd)
{
	if (cmd.empty())
		return;

	const std::string &first = cmd[0];

	if (first == "." || first == "source")
	{
		sourceAnotherFile(cmd);
	}
	else if (first == "alias")
	{
		alias(cmd);
	}
	else if (first == "cd")
	{
		cd(cmd);
	}
	else if (first.find('/') == std::string::npos)
	{
		// execp
	}
	else
	{
		// exec
	}

}

void ScriptInterpreter::runFromScriptSource(ScriptSource &source, bool isInteractive)
{
	StringList preprocessedCmd;
	Preprocessor preprocessor(context, isInteractive);
	while (preprocessor.getProcessedCmd(source, preprocessedCmd))
	{
		run(preprocessedCmd);
	}
}
