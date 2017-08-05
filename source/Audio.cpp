#include <Audio.h>

Audio::Audio()
{
}


Audio::~Audio()
{
}

Audio Audio::instance_;

Audio& Audio::instance() {
	return instance_;
}

