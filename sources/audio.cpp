#include "audio.hpp"

Audio::Audio()
{
}

Audio::~Audio()
{
}

static float audioBuffer[256];
static float audioTime = 0;

void Audio::fillBuffer(exo::sint16* pBuffer, exo::uint numSamples)
{
	audioTime += numSamples / 44100.f;
	while(numSamples > 0)
	{
		int toRender = numSamples > 256 ? 256 : numSamples;
		numSamples -= toRender;		
		
		m_synth.render(audioBuffer, toRender);

		float* pRendered = audioBuffer;
		for(; toRender > 0; toRender--)
		{
			exo::sint16 v = (exo::sint16)(*pRendered++ * 32767);
			*pBuffer++ = v;
			*pBuffer++ = v;
		}		
	}
}
