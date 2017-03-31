#include "stdio.h"
#include "Ambisonics.h"
//#include "wav.hpp"
#include "Wave.h"
int test(){
	// Generation of mono test signal
	float sinewave[512];
	for (int ni = 0; ni < 512; ni++){
		sinewave[ni] = (float)sin((ni / 128.f) * (M_PI * 2));
	}

	// CBFormat as 1st order 3D, and 512 samples
	CBFormat myBFormat(1, true, 512);

	// Ambisonic encoder, also 3rd order 3D
	CAmbisonicEncoder myEncoder;

	//Set test signal's position in the soundfield
	PolarPoint position;
	position.fAzimuth = 0;
	position.fElevation = 0;
	position.fDistance = 5;
	myEncoder.SetPosition(position);
	myEncoder.Refresh();

	// Encode test signal into BFormat buffer
	myEncoder.Process(sinewave, 512, &myBFormat);

	// Ambisonic decoder, also 1st order 3D, for a 5.0 setup
	CAmbisonicDecoder myDecoder;

	// Allocate buffers for speaker feeds
	float** ppfSpeakerFeeds = new float*[5];
	for (int niSpeaker = 0; niSpeaker < 5; niSpeaker++)
	{
		ppfSpeakerFeeds[niSpeaker] = new float[512];
	}

	// Decode to get the speaker feeds
	myDecoder.Process(&myBFormat, 512, ppfSpeakerFeeds);

	// De-allocate speaker feed buffers
	for (int niSpeaker = 0; niSpeaker < 5; niSpeaker++)
	{
		delete[] ppfSpeakerFeeds[niSpeaker];
	}
	delete[] ppfSpeakerFeeds;

	return 0;
}

int readWav(){
	CWave wave1;
	int ret = wave1.Load("test_2_channel.wav");
	if (ret == -1){
		return -1;
	}
	wave1.ToFloatFormat();
	wave1.GetInfo();
	// CBFormat as 1st order 3D, and 512 samples
	CBFormat myBFormat(1, true, wave1.GetSampleCount());

	// Ambisonic encoder, also 3rd order 3D
	CAmbisonicEncoder myEncoder;

	//Set test signal's position in the soundfield
	PolarPoint position;
	position.fAzimuth = 90;
	position.fElevation = 0;
	position.fDistance = 5;
	myEncoder.SetPosition(position);
	myEncoder.Refresh();

	int sampleCount = wave1.GetSampleCount();
	myEncoder.ProcessWithRotation(wave1.GetDataFloat(), sampleCount, &myBFormat, 0);
	float* pMyBFormatData = (float*)malloc( sampleCount * 4 * sizeof(float));

	myBFormat.ExtractStream(pMyBFormatData + 0 * sampleCount, 0, sampleCount);
	myBFormat.ExtractStream(pMyBFormatData + 1 * sampleCount, 1, sampleCount);
	myBFormat.ExtractStream(pMyBFormatData + 2 * sampleCount, 2, sampleCount);
	myBFormat.ExtractStream(pMyBFormatData + 3 * sampleCount, 3, sampleCount);

	int channel = 4;
	wave1.ToWordFormat(pMyBFormatData, sampleCount, channel);
	wave1.Save("copy_Radio.wav", 4);
	free(pMyBFormatData);
	return 0;
}

int saveWav(){
// 	int seconds = 10;
// 	int channels = 4;
// 
// 	// Write WAvÎÄ¼þ
// 	Wave_header header(channels, 48000, 16);
// 
// 
// 	uint32_t length = header.fmt_data->sample_per_sec * seconds * channels * header.fmt_data->bits_per_sample / 8;
// 	uint8_t *data = new uint8_t[length];
// 
// 	for (int j = 0; j < header.fmt_data->sample_per_sec * seconds; j ++){
// 		data[(j * 8) + 0] = 0;		
// 		data[(j * 8) + 1] = 1;
// 		data[(j * 8) + 2] = 2;
// 		data[(j * 8) + 3] = 3;
// 		data[(j * 8) + 4] = 4;
// 		data[(j * 8) + 5] = 5;
// 		data[(j * 8) + 6] = 6;
// 		data[(j * 8) + 7] = 7;
// 	}
// 	CWaveFile::write("test1.wav", header, data, length);
	return 0;
}

int main(){
	readWav();
	saveWav();
	//test();
	getchar();
	return 0;
}
