#ifndef STREAM_SOURCE_H
#define STREAM_SOURCE_H


#include "script_source.h"
#include <istream>
#include <string>

class StreamSource : public ScriptSource
{
public:
	StreamSource();
	explicit StreamSource(std::istream &is);

//	bool getLine(std::string &str) override;
	bool getChar(char &c) override;

	std::istream &getStream() { return *stream; }
	const std::istream &getStream() const { return *stream; }

	void setStream(std::istream &is);

protected:
	std::istream *stream;

};


#endif //STREAM_SOURCE_H
