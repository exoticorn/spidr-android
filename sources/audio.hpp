#ifndef _AUDIO_HPP
#define _AUDIO_HPP

#include "fxsynth.hpp"

class Audio
{
public:
	Audio();
	~Audio();
	
private:

	static void callback(void *pUserData, unsigned char *pStream, int len);
	
	FxSynth m_synth;
};

#endif // _AUDIO_HPP

