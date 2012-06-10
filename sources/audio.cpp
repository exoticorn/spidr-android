#include "audio.hpp"
#include "SDL/SDL.h"
#include <stdlib.h>
#include <stdio.h>

Audio::Audio()
{
	SDL_AudioSpec spec;
	spec.freq = 44100;
	spec.format = AUDIO_S16SYS;
	spec.channels = 0;
	spec.samples = 512;
	spec.callback = &Audio::callback;
	spec.userdata = &m_synth;
	
	if(SDL_OpenAudio(&spec, NULL) < 0)
	{
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(1);
	}
	
	SDL_PauseAudio(0);
}

Audio::~Audio()
{
	SDL_PauseAudio(1);
}

static float audioBuffer[256];

void Audio::callback(void* pUserData, unsigned char* pStream, int len)
{
	signed short* pSamples = (signed short*)pStream;
	int numSamples = len / 2;
	
	while(numSamples > 0)
	{
		int toRender = numSamples > 256 ? 256 : numSamples;
		numSamples -= toRender;		
		
		((FxSynth*)pUserData)->render(audioBuffer, toRender);

		float* pRendered = audioBuffer;
		for(; toRender > 0; toRender--)
		{
			*pSamples++ = *pRendered++ * 32767;
		}		
	}
}
