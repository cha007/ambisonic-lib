#ifndef _AMBISONIC_MICROPHONE_H
#define	_AMBISONIC_MICROPHONE_H

#include "AmbisonicSource.h"
#include "BFormat.h"

/// Ambisonic microphone

/** This is a microphone class. It is similar to ::CAmbisonicSpeaker, with the
	addition of having directivity control. */

class CAmbisonicMicrophone : public CAmbisonicSource
{
public:
	CAmbisonicMicrophone();
	~CAmbisonicMicrophone();
	/**
		Recalculate coefficients, and apply normalisation factors.
	*/
	void Refresh();
	/**
		Decode B-Format to speaker feed.
	*/
	void Process(CBFormat* pBFSrc, AmbUInt nSamples, AmbFloat* pfDst);
	/**
		Set the microphone's directivity.
	*/
	void SetDirectivity(AmbFloat fDirectivity);
	/**
		Get the microphone's directivity.
	*/
	AmbFloat GetDirectivity();

protected:
	AmbFloat m_fDirectivity;
};

#endif // _AMBISONIC_MICROPHONE_H