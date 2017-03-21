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
#include <cassert>
#include <regex>
#include <iomanip>
#include "cinclude/cunistd.h"
#include "history.h"


ScriptInterpreter::ScriptInterpreter() {}

int ScriptInterpreter::executeString(const std::string &cmd)
{
	StringList preprocessedCmd = Preprocessor(context).process(cmd);
	run(preprocessedCmd);
	return context.getLastExitCode();
}

int ScriptInterpreter::interactive(const std::string &initCmd)
{
//	sourceStack.push(InteractiveSource(context));
	context.initInteractiveContext();
	if (!initCmd.empty())
		executeString(initCmd);
	InteractiveSource source(context);
	runFromScriptSource(source, true);
	int result = context.getLastExitCode();
	std::cout << "exit" << std::endl;
	return result;
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
	int exitCode = 0;
	if (args.size() <= 1)
	{
		auto dict = context.getAliasDictionary();
		for (const auto &pair : dict)
		{
			std::cout << "alias " << pair.first << "='"
					  << pair.second <<'\'' << std::endl;
		}
	}
	else
	{
		for (unsigned i = 1; i < args.size(); ++i)
		{
			auto arg = Utils::splitOnce(args[i], '=');
			assert(arg.size() >= 1);
			if (std::regex_match(arg[0], Utils::getIdentifierRegex()))
			{
				if (arg.size() > 1)
					context.getAliasDictionary()[arg[0]] = arg[1];
				else
				{
					auto it = context.getAliasDictionary().find(arg[0]);
					if (it == context.getAliasDictionary().end())
					{
						Log::LogError("alias: " + arg[0] + ": not found");
						exitCode = 1;
					}
					else
					{
						std::cout << "alias" << it->first << "='"
								  << it->second << '\'' << std::endl;
					}
				}
			}
			else
			{
				Log::LogError("alias: " + arg[0] + ": invalid alias");
				exitCode = 1;
			}
		}
	}
//	else if (args[1].find('=') == std::string::npos)
//	{
//		auto it = context.getAliasDictionary().find(args[1]);
//		if (it == context.getAliasDictionary().end())
//		{
//			Log::LogError("alias: " + args[1] +": not found");
//			context.setLastExitCode(1);
//			return;
//		}
//		else
//		{
//			std::cout << "alias " << it->first << "='"
//					  << it->second << '\'' << std::endl;
//		}
//	}
//	else
//	{
//		unsigned split = 0;
//		while (args[1][split] != '=')
//			++split;
//		std::string name(args[1].begin(), args[1].begin() + split);
//		if (!std::regex_match(name, std::regex("[a-zA-Z_][a-zA-Z0-9_]*")))
//		{
//			Log::LogError("alias: " + name + ": invalid alias");
//			context.setLastExitCode(1);
//			return;
//		}
//		else
//		{
//			context.getAliasDictionary()[name] =
//					std::string(args[1].begin() + split + 1, args[1].end());
//		}
//	}
	context.setLastExitCode(exitCode);
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
		Log::LogErrorByErrno("cd");
	}
	else
		context.setLastExitCode(0);
}

void ScriptInterpreter::exportVar(const StringList &args)
{
	int exitCode = 0;
	for (unsigned i = 1; i < args.size(); ++i)
	{
		auto arg = Utils::splitOnce(args[i], '=');
		assert(arg.size() >= 1);
		if (std::regex_match(arg[0], Utils::getIdentifierRegex()))
		{
			context.exportVar(arg[0]);
		}
		else
		{
			Log::LogError("export: " + arg[0] + ": not a valid identifier");
			exitCode = 1;
		}
	}

	context.setLastExitCode(exitCode);
}

void ScriptInterpreter::eval(const StringList &args)
{
	int exitCode = 0;
	auto arg = Utils::splitOnce(args[1], '=');
	assert(arg.size() >= 1);
	if (arg.size() > 1 && std::regex_match(arg[0], Utils::getIdentifierRegex()))
	{
		context.setVar(arg[0], arg[1]);
	}
	else
	{
		//TODO: run this as a normal command line
		Log::LogError("eval: feature not implemented");
		exitCode = 1;
	}

	context.setLastExitCode(exitCode);
}

void ScriptInterpreter::history(const StringList &args)
{
	auto range = History::getAll();
	auto it = range.first;
	auto end = range.second;
	int count = range.second - range.first;
	int width = 1;
	while (count /= 10)
		++width;
	for (; it != end; ++it)
		std::cout << std::setw(width) << it - range.first + 1 << ' ' << *it << std::endl;
}

void ScriptInterpreter::createProcess(const char *path, const char * const *args, bool noawait)
{
	pid_t pid = ::fork();
	if (pid)
	{
		if (!noawait)
		{
			int status;
			::waitpid(pid, &status, 0);
			context.setLastExitCode(WEXITSTATUS(status));
		}
		else
			context.setLastExitCode(0);
	}
	else
	{
		Environment::getInstance()->doUntrap();
		execvp(path, (char * const *)args);
		int err = errno;
		Log::LogErrorByErrno(path);
		exit(err);
	}
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
	else if (first == "export")
	{
		exportVar(cmd);
	}
	else if (first == "eval")
	{
		eval(cmd);
	}
	else if (first == "history")
	{
		history(cmd);
	}
	else
	{
		unsigned size = cmd.size();
		bool noawait = cmd.back() == "&";
		if (noawait)
			--size;
		if (!size)
		{
			Log::LogError("unexpected symbol &");
			context.setLastExitCode(2);
			return;
		}
		typedef const char *pchar;
		auto args = new pchar[size + 1];
		for (unsigned i = 0; i < size; ++i)
			args[i] = cmd[i].c_str();
		args[size] = nullptr;
		createProcess(args[0], args, noawait);
		delete[] args;
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
