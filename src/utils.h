#ifndef UTILS_H
#define UTILS_H


#include <unordered_map>
#include <string>
#include <deque>
#include <iostream>
#include "cinclude/cerrno.h"
#include <cstdio>


typedef std::unordered_map<std::string, std::string> StringMap;

typedef std::deque<std::string> StringList;


class Log
{
public:

	// Static classes do not need any constructor
	Log() = delete;

	// TODO: Finish Log class
	static void LogError(const std::string &content)
	{
		std::string str = "-myonsh: " + content +"\n";
		std::cerr << str;
	}

	static void LogErrorByErrno(const char *s)
	{
		::perror((std::string("-myonsh: ") + s).c_str());
	}

};

class Utils
{
public:

	//.
	Utils() = delete;


};


#endif //UTILS_H
