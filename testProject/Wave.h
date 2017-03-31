#pragma once
#include <windows.h>
#include <stdio.h>
#pragma pack(1)
typedef struct __WAVEDESCR
{
	BYTE riff[4];
	DWORD size;
	BYTE wave[4];

} _WAVEDESCR, *_LPWAVEDESCR;

typedef struct __WAVEFORMAT
{
	BYTE id[4];
	DWORD size;
	SHORT format;
	SHORT channels;
	DWORD sampleRate;
	DWORD byteRate;
	SHORT blockAlign;
	SHORT bitsPerSample;

} _WAVEFORMAT, *_LPWAVEFORMAT;
#pragma pack()


class CWave
{
public:
	CWave(void);
	virtual ~CWave(void);

public:
	// Public methods
	BOOL Load(const char* filePath);
	BOOL Save(const char* filePath, int channel);
	BOOL Mix(CWave& wave);	
	BOOL ToFloatFormat();
	BOOL ToWordFormat(float* pMyBFormatData, int sampleCount, int channel);

	BOOL IsValid()				{return (m_lpData != NULL);}
	LPBYTE GetData()			{return m_lpData;}
	float* GetDataFloat()		{return m_lpDataFloat;}
	DWORD GetSize()				{return m_dwSize;}
	SHORT GetChannels()			{return m_Format.channels;}
	DWORD GetSampleRate()		{return m_Format.sampleRate;}
	SHORT GetBitsPerSample()	{return m_Format.bitsPerSample;}
	void GetInfo(){
		printf("wave_format:\n");
		printf("channels: %d\n", GetChannels());
		printf("sample_rate:%d\n", GetSampleRate());
		printf("sample_bits:%d\n", GetBitsPerSample());
		printf("size:%d\n", GetSize());
		printf("size:%d\n", m_Descriptor.size);
	}
	DWORD GetSampleCount(){
		int result = GetSize() / (GetBitsPerSample() / 8) / GetChannels();
		return result;
	}

private:
	BOOL Close();

private:
	// Private members
	_WAVEDESCR m_Descriptor;
	_WAVEFORMAT m_Format;
	LPBYTE m_lpData;
	float* m_lpDataFloat;
	LPBYTE m_lpDataOut;
	DWORD m_dwSize;
};
