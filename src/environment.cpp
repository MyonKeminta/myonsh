#include "environment.h"
#include "cinclude/cunistd.h"
#include <string>
#include <stack>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "utils.h"


Environment *Environment::instance = nullptr;

Environment *Environment::getInstance()
{
	return instance;
}

void Environment::initWithArgs(const std::vector<std::string> &args)
{
	if (instance)
	{
		Log::LogError("Inner error: initialize environment the second time.");
	}
	else
	{
		instance = new Environment(args);
	}
}

//Environment::Environment()
//{

//}

Environment::Environment(const std::vector<std::string> &args)
	: args(args)
{

}

std::string Environment::getEnv(const char *name) const
{
	return ::getenv(name);
}

bool Environment::hasEnv(const char *name) const
{
	return ::getenv(name);
}

bool Environment::tryGetEnv(const char *name, std::string &dest) const
{
	const char *result = ::getenv(name);
	dest = result ? result : "";
	return result;
}

void Environment::setEnv(const char *name, const char *value)
{
	::setenv(name, value, 1);
}

void Environment::unsetEnv(const char *name)
{
	::unsetenv(name);
}

StringList Environment::getSystemPath() const
{
	std::string path = getEnv("PATH");
	auto begin = path.begin();
	auto end = path.end();
	StringList list;
	while (true)
	{
		auto split = std::find(begin, end, ':');

		// Skip invalid path.
		if(*split == '/')
			list.emplace_back(begin, split);

		if(split == end)
			break;

		++split;
	}
	return list;
}

int Environment::chdir(const char *path)
{
	if(::chdir(path))
	{
		int e = errno;
		Log::LogErrorByErrno("cd");
		return e;
	}
	setEnv("OLDPWD", getEnv("PWD").c_str());
	setEnv("PWD", path);
	return 0;
}


int Environment::getUid() const
{
	return ::getuid();
}

int Environment::getEuid() const
{
	return ::geteuid();
}

std::string Environment::getPwd() const
{
//	unsigned size = 64;
//	char *str = new char[size];
//	while (!::getcwd(str, size))
//	{
//		delete str;
//		str = new char[size*=2];
//	}

//	std::string result(str);
//	delete str;
//	return result;
	return getEnv("PWD");
}

std::string Environment::getHostName() const
{
	unsigned size = 64;
	char *str = new char[size];
	while (!::gethostname(str, size))
	{
		delete str;
		str = new char[size*=2];
	}

	std::string result(str);
	delete str;
	return result;
}

std::string Environment::getUserName() const
{
	return getEnv("USER");
}

std::string Environment::getHome() const
{
	return getEnv("HOME");
}


bool Environment::isInputRedirected() const
{
	return !::isatty(0);
}


const std::vector<std::string> &Environment::getArgs() const
{
	return args;
}






