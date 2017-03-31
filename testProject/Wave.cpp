#include "Wave.h"
#include "math.h"
#include <stdio.h>

CWave::CWave(void)
{
	// Init members
	memset(&m_Descriptor, 0, sizeof(_WAVEDESCR));
	memset(&m_Format, 0, sizeof(_WAVEFORMAT));
	m_lpData = NULL;
	m_lpDataFloat = NULL;
	m_lpDataOut = NULL;
	m_dwSize = 0;
}

CWave::~CWave(void)
{
	// Close output device
	Close();
}

BOOL CWave::Close()
{
	BOOL bResult = TRUE;

	// Check for valid sound data
	if (IsValid())
	{
		// Clear sound data buffer
		free(m_lpData);
		m_lpData = NULL;
		free(m_lpDataFloat);
		m_lpDataFloat = NULL;
		free(m_lpDataOut);
		m_lpDataOut = NULL;
		m_dwSize = 0;
	}

	return bResult;
}

BOOL CWave::Load(const char* filePath)
{
	// Close output device
	Close();

	// Load .WAV file
	FILE* file = NULL;
	fopen_s(&file, filePath, ("rb"));
	if (file != NULL)
	{
		// Read .WAV descriptor
		fread(&m_Descriptor, sizeof(_WAVEDESCR), 1, file);

		// Check for valid .WAV file
		if (strncmp((LPCSTR)m_Descriptor.wave, "WAVE", 4) == 0)
		{
			// Read .WAV format
			fread(&m_Format, sizeof(_WAVEFORMAT), 1, file);
			
			if (m_Format.channels != 1 && m_Format.channels != 2){
				printf("audio channel is %d, pls use mono or 2_channel audio.\n", m_Format.channels);
				return -1;
			}

			// Check for valid .WAV file
			if ((strncmp((LPCSTR)m_Format.id, "fmt", 3) == 0) && (m_Format.format == 1))
			{
				// Read next chunk
				BYTE id[4];
				DWORD size;
				fread(id, sizeof(BYTE), 4, file);
				fread(&size, sizeof(DWORD), 1, file);
				DWORD offset = ftell(file);

				// Read .WAV data
				LPBYTE lpTemp = (LPBYTE)malloc(m_Descriptor.size*sizeof(BYTE));
				while (offset < m_Descriptor.size)
				{
					// Check for .WAV data chunk
					if (strncmp((LPCSTR)id, "data", 4) == 0)
					{
						if (m_lpData == NULL)
							m_lpData = (LPBYTE)malloc(size*sizeof(BYTE));
						else
							m_lpData = (LPBYTE)realloc(m_lpData, (m_dwSize+size)*sizeof(BYTE));
						fread(m_lpData + m_dwSize, sizeof(BYTE), size, file);						
						m_dwSize += size;
					}
					else
						fread(lpTemp, sizeof(BYTE), size, file);

					// Read next chunk
					fread(id, sizeof(BYTE), 4, file);
					fread(&size, sizeof(DWORD), 1, file);
					offset = ftell(file);
				}
				free(lpTemp);
			}
		}

		// Close .WAV file
		fclose(file);
	}
	else{
		printf("file not exist\n");
		return -1;
	}

	return 0;
}

BOOL CWave::Save(const char* filePath, int channel)
{
	BOOL bResult = FALSE;

	// Save .WAV file
	FILE* file = NULL;
	fopen_s(&file, filePath, "wb");
	if (file != NULL)
	{
		// Save .WAV descriptor
		DWORD size = m_dwSize / m_Format.channels * channel;

		m_Format.channels = channel;
		m_Descriptor.size = size;
		fwrite(&m_Descriptor, sizeof(_WAVEDESCR), 1, file);

		// Save .WAV format
		fwrite(&m_Format, sizeof(_WAVEFORMAT), 1, file);

		// Write .WAV data
		BYTE id[4] = {'d', 'a', 't', 'a'};
		fwrite(id, sizeof(BYTE), 4, file);
		fwrite(&size, sizeof(DWORD), 1, file);
		fwrite(m_lpDataOut, sizeof(BYTE), size, file);
		bResult = TRUE;

		// Close .WAV file
		fclose(file);
		printf("save finish\n");
	}
	else{
		printf("output save failed\n");
	}

	return bResult;
}

BOOL CWave::Mix(CWave& wave)
{
	BOOL bResult = FALSE;

	// Check for valid sound data
	if (IsValid() && wave.IsValid())
	{
		// Check for valid sound format
		if ((m_Format.channels == wave.GetChannels()) && (m_Format.sampleRate == wave.GetSampleRate()) && (m_Format.bitsPerSample == wave.GetBitsPerSample()))
		{
			// Mix .WAVs
			long sampleSize = min(m_dwSize, wave.GetSize()) / (m_Format.bitsPerSample >> 3);
			switch (m_Format.bitsPerSample)
			{
				case 8:
					{
						LPBYTE lpSrcData = wave.GetData();
						LPBYTE lpDstData = m_lpData;
						float gain = log10(20.0f);
						for (long i=0; i<sampleSize; i++)
						{
							*lpDstData = (BYTE)(((*lpSrcData+*lpDstData)>>1)*gain);
							lpSrcData++;
							lpDstData++;
						}
					}
					break;

				case 16:
					{
						LPWORD lpSrcData = (LPWORD)wave.GetData();
						LPWORD lpDstData = (LPWORD)m_lpData;
						for (long i=0; i<sampleSize; i++)
						{
							float sample1 = (*lpSrcData - 32768) / 32768.0f;
							float sample2 = (*lpDstData - 32768) / 32768.0f;
							if (fabs(sample1*sample2) > 0.25f)
								*lpDstData = (WORD)(*lpSrcData + *lpDstData);
							else
								*lpDstData = fabs(sample1) < fabs(sample2) ? *lpSrcData : *lpDstData;
							lpSrcData++;
							lpDstData++;
						}
					}
					break;
			}
			bResult = TRUE;
		}
	}

	return bResult;
}

BOOL CWave::ToFloatFormat(){
	BOOL bResult = FALSE;
	// Check for valid sound data
	if (IsValid()){
		long sampleSize = m_dwSize / (m_Format.bitsPerSample >> 3) / m_Format.channels;
		m_lpDataFloat = (float*)malloc(sampleSize * sizeof(float));
		short* lpDstData = (short*)m_lpData;
		float* lpFloatData = m_lpDataFloat;
		for (long i = 0; i < sampleSize; i++){
			float sample = (*lpDstData) / 32768.0f;
			*lpFloatData = sample;
			lpDstData += m_Format.channels;
			lpFloatData++;
		}
		bResult = TRUE;
	}
	return bResult;
}
BOOL CWave::ToWordFormat(float* pMyBFormatData, int sampleCount, int channel){
	BOOL bResult = FALSE;
	// Check for valid sound data
	if (IsValid()){
		int niChannel = 0;
		int niSample = 0;
		float* lpFloatData = pMyBFormatData;

		long sampleSize = m_dwSize / (m_Format.bitsPerSample >> 3) / m_Format.channels;
		m_lpDataOut = (LPBYTE)malloc(sampleSize * sizeof(BYTE) * channel * 2);
		LPWORD lpDstData = (LPWORD)m_lpDataOut;

		for (niSample = 0; niSample < sampleCount; niSample++){
			for (niChannel = 0; niChannel < channel; niChannel++){
				lpDstData[niSample * channel + niChannel] = (short)(lpFloatData[sampleCount * niChannel + niSample] * 32768.0f);
// 				if (niChannel == 0 && niSample > 100 && niSample < 120){
// 					printf("[%d][in %0.4f out %d]\n", niSample, lpFloatData[sampleCount * niChannel + niSample], lpDstData[niSample * 4 + niChannel]);
// 				}
			}
		}
		bResult = TRUE;
	}
	return bResult;
}
