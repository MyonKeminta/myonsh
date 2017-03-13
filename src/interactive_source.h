#ifndef INTERACTIVE_SOURCE_H
#define INTERACTIVE_SOURCE_H

#include <myonsh_config.h>
#include <string>
#include "script_source.h"

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
	InteractiveSource();

	bool getLine(std::string &str) override
	{
		printPrompt();
		return StreamSource::getLine(str);
	}


private:
	void printPrompt();
};



//#endif //HAS_GNU_READLINE


#endif //INTERACTIVE_SOURCE_H