#ifndef PREPROCESSOR
#define PREPROCESSOR


#include "context.h"
#include "script_source.h"

class Preprocessor : ScriptSource
{
public:

	Preprocessor(Context &context, ScriptSource &source);

	bool getChar(char &c) override;



private:

	ScriptSource *source;

	Context *context;

};



#endif //PREPROCESSOR
