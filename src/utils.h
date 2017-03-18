#ifndef UTILS_H
#define UTILS_H


#include <unordered_map>
#include <string>
#include <deque>
#include <iostream>
#include "cinclude/cerrno.h"
#include <cstdio>
#include <regex>


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

	static StringList splitOnce(const std::string &str, char splitter)
	{
		unsigned pivot = 0;
		while (pivot < str.size() && str[pivot] != splitter)
			++pivot;
		StringList result;
		result.emplace_back(str.begin(), str.begin() + pivot);
		if (pivot < str.size())
			result.emplace_back(str.begin() + pivot + 1, str.end());
		return result;
	}

	static const std::regex &getIdentifierRegex()
	{
		static const std::regex regex("[_a-zA-Z][_0-9a-zA-Z]*");
		return regex;
	}

};


#endif //UTILS_H
