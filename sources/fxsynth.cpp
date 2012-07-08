#include "fxsynth.hpp"
#include "exo/base/functions.hpp"
#include <math.h>
#include <stdlib.h>

const FxSynth::SynthCode fxStopCode = { FxSynth::EndOfCode, 0, 0, 0, 0, 0 };
FxSynth* FxSynth::m_pInstance = 0;

FxSynth::FxSynth()
{
	m_osciPhase = 0;
	m_filterLow = m_filterBand = 0;
	m_samplesLeft = 0;
	m_pCurrentCode = &fxStopCode;
	m_pInstance = this;
	m_dontAbort = false;
	m_freqFactor = 1;
}

void FxSynth::playSfx(SynthCode* pCode, float volume, bool dontAbort)
{
	m_pInstance->m_mutex.lock();
	if(!m_pInstance->m_dontAbort)
	{
		m_pInstance->m_pCurrentCode = pCode;
		m_pInstance->m_volume = volume;
		m_pInstance->m_dontAbort = dontAbort;
		m_pInstance->m_freqFactor = powf(2.0f, ((rand() & 255) - 128) / 512.0f);
	}
	m_pInstance->m_mutex.unlock();
}

void FxSynth::render(float* pBuffer, int numSamples)
{
	while(numSamples > 0)
	{
		m_mutex.lock();
		if(m_samplesLeft == 0)
		{
			if(m_pCurrentCode->osciType != EndOfCode)
			{
				m_pCurrentCode++;
			}
			else
			{
				m_dontAbort = false;
			}
			m_samplesLeft = 44100/60;
		}
		
		const SynthCode& code = *m_pCurrentCode;
		float volume = m_volume;
		float freqFactor = m_freqFactor;
		m_mutex.unlock();

		int samplesToRender = m_samplesLeft < numSamples ? m_samplesLeft : numSamples;
		
		for(int i = 0; i < samplesToRender; i++)
		{
			float sample = 0;
			switch(code.osciType)
			{
			case Osci_Rect:
				sample = m_osciPhase < code.duty ? 1 : -1;
				break;
			case Osci_Wave:
				if(m_osciPhase < code.duty)
				{
					sample = (m_osciPhase / code.duty) * 2 - 1;
				}
				else
				{
					sample = ((1 - m_osciPhase) / (1 - code.duty)) * 2 - 1;
				}
				break;
			case EndOfCode:
				break;
			}
			
			m_osciPhase += code.osciInc * freqFactor;
			if(m_osciPhase >= 1)
			{
				m_osciPhase -= 1;
			}
			
			sample *= code.osciVolume;
			
			m_rngState = m_rngState * 4637237 + 3763723;
			
			float noise = (m_rngState / 2147483648.0f) - 1;
			sample += noise * code.noiseVolume;
			
			m_filterLow += code.filterFreq * freqFactor * m_filterBand;
			float high = sample - m_filterLow - m_filterBand;
			m_filterBand += code.filterFreq * freqFactor * high;
			
			*pBuffer++ = exo::clamp(-1.0f, 1.0f, m_filterLow * volume);
		}
		
		m_samplesLeft -= samplesToRender;
		numSamples -= samplesToRender;
	}
}
