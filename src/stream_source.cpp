#include "stream_source.h"
#include <istream>
#include <string>

StreamSource::StreamSource() : stream(nullptr) {}

StreamSource::StreamSource(std::istream &is) : stream(&is) {}

bool StreamSource::getLine(std::string &str)
{
	str.clear();
	return std::getline(*stream, str).good();
}

void StreamSource::setStream(std::istream &is)
{
	stream = &is;
}
