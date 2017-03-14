#ifndef SCRIPT_INTERPRETER_H
#define SCRIPT_INTERPRETER_H


#include "context.h"
#include <string>
#include <stack>
#include "utils.h"
#include "script_source.h"
#include "interactive_source.h"
#include "stream_source.h"
#include "cinclude/cunistd.h"


class ScriptInterpreter
{
public:

	ScriptInterpreter();

	virtual ~ScriptInterpreter() {}

	void executeString(const std::string &cmd);

	void interactive();
	
private:

//	void run();

	StringList currentCommand;

	Context context;

	std::stack<ScriptSource> sourceStack;

};



#endif //SCRIPT_INTERPRETER_H
