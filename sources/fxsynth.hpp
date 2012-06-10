#ifndef _FX_SYNTH_HPP
#define _FX_SYNTH_HPP

class FxSynth
{
public:
	enum OsciType { Osci_Rect, Osci_Wave, EndOfCode };

	struct SynthCode
	{
		OsciType osciType;
		float osciInc;
		float osciVolume;
		float duty;
		float noiseVolume;
		float filterFreq;
	};
	
	FxSynth();
	
	static void playSfx(SynthCode* pCode, float volume = 1, bool dontAbort = false);
	void render(float* pBuffer, int size);
	
private:

	static FxSynth* m_pInstance;

	float m_osciPhase;
	float m_filterLow;
	float m_filterBand;
	int m_samplesLeft;
	unsigned int m_rngState;
	const SynthCode* m_pCurrentCode;
	float m_volume;
	bool m_dontAbort;
};

#endif // _FX_SYNTH_HPP

