#ifndef _AUDIO_HPP
#define _AUDIO_HPP

#include "fxsynth.hpp"
#include "exo/base/types.hpp"

class Audio
{
public:
	Audio();
	~Audio();
	
	void fillBuffer(exo::sint16 *pBuffer, exo::uint numSamples);

private:
	
	FxSynth m_synth;
};

#endif // _AUDIO_HPP

