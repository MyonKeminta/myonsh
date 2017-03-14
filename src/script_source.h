#ifndef SCRIPT_SOURCE_H
#define SCRIPT_SOURCE_H

#include <string>

class ScriptSource
{
public:
	virtual ~ScriptSource() {}

	//get one line from the source.
	//param: str: target to store the line
	//return: is gettingline successful.
//	virtual bool getLine(std::string &str) = 0;
	virtual bool getChar(char &c) = 0;

	void source(ScriptSource &another);
};



#endif //SCRIPT_SOURCE_H
