#ifndef WORD_DIVIDER_H
#define WORD_DIVIDER_H


#include "context.h"
#include "utils.h"



class WordDivider
{
public:

	explicit WordDivider(Context &context);

	virtual ~WordDivider() {}

	virtual StringList getLine();


private:

	Context *context;

};



#endif //WORD_DIVIDER_H
