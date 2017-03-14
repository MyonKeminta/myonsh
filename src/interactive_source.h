#ifndef INTERACTIVE_SOURCE_H
#define INTERACTIVE_SOURCE_H

#include <myonsh_config.h>
#include <string>
#include "script_source.h"
#include "context.h"

//#ifdef HAS_GNU_READLINE



//#include "cinclude/creadline.h"

// class InteractiveSource : public ScriptSource
// {
// public:
// 	InteractiveSource();
	
// 	bool getLine(std::string &str) override;
// };



// #else //HAS_GNU_READLINE



#include <iostream>
#include "stream_source.h"


class InteractiveSource : public StreamSource
{
public:
	explicit InteractiveSource(Context &context)
		: InteractiveSource(context, std::cin) {}

	InteractiveSource(Context &context, std::istream &is)
		: context(&context), StreamSource(is) {}

//	bool getLine(std::string &str) override
//	{
//		printPrompt();
//		if (StreamSource::getLine(str))
//		{
//			tryExpandAlias(str);
//			return true;
//		}
//		return false;
//	}
	bool getChar(char &c) override
	{
		if(newLine)
			printPrompt();

		bool result = StreamSource::getChar(c);
		newLine = '\n' == c;
		return result;
	}


private:
	void printPrompt();

	void tryExpandAlias(std::stirng &str);

	bool newLine = true;

	Context *context;
};



//#endif //HAS_GNU_READLINE


#endif //INTERACTIVE_SOURCE_H
