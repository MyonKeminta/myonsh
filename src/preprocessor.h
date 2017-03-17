#ifndef PREPROCESSOR
#define PREPROCESSOR


#include "context.h"
#include "script_source.h"
#include "utils.h"
#include <string>
#include <stack>


class Preprocessor/* : public ScriptSource*/
{
public:

	Preprocessor(Context &context,  bool expandAlias = false);


//	bool getChar(char &c) override;

	bool getProcessedCmd(ScriptSource &source, StringList &dest);

	StringList process(const std::string &_cmd);


private:

	ScriptSource *source;

	Context *context;

	bool expandAlias;

	std::stack<char> puttedBack;


//	std::string getLine();
	bool getLine(std::string &str);

	std::string readWithEscapesAndMacros(const std::string &end);

	bool getChar(char &c);

	void putBack(char c);


};



#endif //PREPROCESSOR
