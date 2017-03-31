#ifndef _AMBISONIC_ENCODER_H
#define	_AMBISONIC_ENCODER_H

#include "AmbisonicSource.h"
#include "BFormat.h"

/// Ambisonic encoder.

/** This is a basic encoder that only takes the source's azimuth an elevation 
	into account. If distance cues are going to be used, then use 
	CAmbisonicEncoderDist instead. */

class CAmbisonicEncoder : public CAmbisonicSource
{
public:
	CAmbisonicEncoder();
	~CAmbisonicEncoder();
    /**
		Re-create the object for the given configuration. Previous data is
		lost. Returns true if successful.
	*/
	virtual bool Create(AmbUInt nOrder, AmbBool b3D, AmbUInt nMisc);
	/**
		Recalculate coefficients, and apply normalisation factors.
	*/
	void Refresh();
	/**
		Encode mono stream to B-Format.
	*/
	void Process(AmbFloat* pfSrc, AmbUInt nSamples, CBFormat* pBFDst);
	/*
		Rotate the mono sound around center
	*/
	void ProcessWithRotation(AmbFloat* pfSrc, AmbUInt nSamples, CBFormat* pfDst, int offset);
};

#endif // _AMBISONIC_ENCODER_H