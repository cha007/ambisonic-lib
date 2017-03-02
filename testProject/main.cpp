#include "stdio.h"
#include "Ambisonics.h"
int test(){
	// Generation of mono test signal
	float sinewave[512];
	for (int ni = 0; ni < 512; ni++){
		sinewave[ni] = (float)sin((ni / 128.f) * (M_PI * 2));
	}

	// CBFormat as 1st order 3D, and 512 samples
	CBFormat myBFormat;

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

int main(){
	test();
	return 0;
}