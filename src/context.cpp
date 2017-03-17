#include "context.h"
#include "utils.h"
#include <string>
#include <stack>
#include "exceptions.h"
#include "environment.h"
#include "config.h"
#include <cctype>


const StringMap &LocalContext::getVariableDictionary() const
{
	return variableDict;
}

StringMap &LocalContext::getVariableDictionary()
{
	return variableDict;
}





Context::Context() : environment(Environment::getInstance())
{
	globalVariableDict["HOSTNAME"] = environment->getHostName();
}

const StringMap &Context::getGlobalVariableDictionary() const
{
	return globalVariableDict;
}

StringMap &Context::getGlobalVariableDictionary()
{
	return globalVariableDict;
}

const StringMap &Context::getFunctionDictionary() const
{
	return functionDict;
}

StringMap &Context::getFunctionDictionary()
{
	return functionDict;
}

const StringMap &Context::getAliasDictionary() const
{
	return aliasDictionary;
}

StringMap &Context::getAliasDictionary()
{
	return aliasDictionary;
}


std::string Context::getPrompt() const
{
	if (status == Ready)
		return parsePrompt(getVar("PS1"));
	else
		return parsePrompt(getVar("PS2"));
}

Context::Status Context::getStatus() const
{
	return status;
}

void Context::setStatus(const Status &value)
{
	status = value;
}



void Context::initInteractiveContext()
{
	setVar("PS1", DEFAULT_PS1);
	setVar("PS2", DEFAULT_PS2);
	setVar("PS3", DEFAULT_PS3);
	setVar("PS4", DEFAULT_PS4);
}



std::string Context::getVar(const std::string &name) const
{
	if (!callStack.empty())
	{
		const StringMap &dict = callStack.top().getVariableDictionary();
		auto it = dict.find(name);
		if (it != dict.end())
			return it->second;
	}
	auto it = globalVariableDict.find(name);
	if (it != globalVariableDict.end())
		return it->second;
	return environment->getEnv(name);
}

bool Context::hasVar(const std::string &name) const
{
	return (!callStack.empty() && callStack.top().getVariableDictionary().count(name)) ||
			globalVariableDict.count(name) || environment->hasEnv(name);
}

void Context::setVar(const std::string &name, const std::string &value)
{
	if (environment->hasEnv(name))
		environment->setEnv(name, value);
	else
		globalVariableDict[name] = value;
}

void Context::unset(const std::string &name)
{
	if (!callStack.empty())
	{
		auto &dict = callStack.top().getVariableDictionary();
		auto it = dict.find(name);
		if (it != dict.end())
		{
			dict.erase(it);
			return;
		}
	}
	auto it = globalVariableDict.find(name);
	if (it != globalVariableDict.end())
		globalVariableDict.erase(it);
	else
		environment->unsetEnv(name);
}

void Context::exportVar(const std::string &name)
{
	auto it = globalVariableDict.find(name);
	if (it != globalVariableDict.end())
	{
		environment->setEnv(name, it->second);
		globalVariableDict.erase(it);
	}
}



std::string Context::parsePrompt(const std::string &ps) const
{
	std::string result;

	for (unsigned i = 0; i < ps.size(); ++i)
	{
		if(ps[i] != '\\')
		{
			result.push_back(ps[i]);
			continue;
		}

		if (i == ps.size() - 1)
		{
			result.push_back(ps[i]);
			continue;
		}

		// Parse escapes.

		++i;
		if (ps[i] == '0') // Oct value
		{
			++i;
			int value = 0;
			for (int digits = 0; digits < 3; ++digits)
			{
				// If non-oct-digit character
				if (ps[i] < '0' || ps[i] > '7')
				{
					// Go back
					--i;
					break;
				}

				value = (value << 3) | (ps[i++] - '0');
			}

			result.push_back(char(value & 0xff));
		}
		else if (ps[i] == 'w') // PWD
		{
			std::string pwd = environment->getPwd();
			std::string home = environment->getHome();
			if(pwd.size() >= home.size() && !pwd.find_first_of(home))
			{
				pwd[home.size() - 1] = '~';
				pwd = pwd.substr(home.size() - 1);
			}
			result.append(pwd);
		}
		else if (ps[i] == 'h') // Host name
		{
			result.append(environment->getHostName());
		}
		else if (ps[i] == 'u') // User name
		{
			result.append(environment->getUserName());
		}
		else if (ps[i] == '$') // Prompt sign
		{
			result.push_back(environment->getEuid() ? '$' : '#');
		}
		else if (ps[i] == '\\') // A backslash
		{
			result.push_back('\\');
		}
		else if (ps[i] == 's')  // Shell Name
		{
			unsigned start = 0;
			const std::string &cmd = environment->getArgs()[0];
			for (unsigned i = 0; i < cmd.size(); ++i)
			{
				if (cmd[i] == '/')
					start = i + 1;
			}
			result.append(cmd.c_str() + start);
		}
		else if (ps[i] == 'v')  // Version
		{
			result.append(std::to_string(VERSION_MAJOR));
			result.push_back('.');
			result.append(std::to_string(VERSION_MINOR));
		}
		else if (ps[i] == 'n') // New line
		{
			result.push_back('\n');
		}
		else if (ps[i] == 'r') // Return
		{
			result.push_back('\r');
		}
		else // Refuse to translate it
		{
			result.push_back('\\');
			result.push_back(ps[i]);
		}

	}

	return result;
}





