#ifndef UTILS_H
#define UTILS_H


#include <unordered_map>
#include <string>
#include <deque>
#include <iostream>

typedef std::unordered_map<std::string, std::string> StringMap;

typedef std::deque<std::string> StringList;


class Log
{
	// TODO: Finish Log class
	static void LogError(const std::string &content)
	{
		std::string str = "-myonsh: " + content +"\n";
		std::cerr << str;
	}

};


#endif UTILS_H
