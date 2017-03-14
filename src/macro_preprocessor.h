#ifndef MACRO_PREPROCESSOR
#define MACRO_PREPROCESSOR


#include "context.h"
#include "script_source.h"

class MacroPreprocessor : ScriptSource
{
public:

	MacroPreprocessor(Context &context, ScriptSource &source);

	bool getChar(char &c) override;



private:

	ScriptSource *source;

	Context *context;

};



#endif //MACRO_PREPROCESSOR
