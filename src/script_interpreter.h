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

	int executeString(const std::string &cmd);

	int interactive(const std::string &initCmd);


	
private:

	void sourceAnotherFile(const StringList &args);

	void alias(const StringList &args);

	void cd(const StringList &args);

	void exportVar(const StringList &args);

	void eval(const StringList &args);

	void createProcess(const char *path, const char *const *args, bool noawait = false);

	void run(const StringList &cmd);

	void runFromScriptSource(ScriptSource &source, bool isInteractive = false);

	StringList currentCommand;

	Context context;

//	std::stack<ScriptSource> sourceStack;

};



#endif //SCRIPT_INTERPRETER_H
