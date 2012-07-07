#include "exo/gameframework/applicationbase.hpp"

namespace exo
{
	void ApplicationBase::fillAudioBuffer(sint16* pBuffer, uint numSamples)
	{
		for(uint i = 0; i < numSamples * 2; ++i)
		{
			pBuffer[i] = 0;
		}
	}
}
