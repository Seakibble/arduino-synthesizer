#pragma once

#include "Oscillator.h"
#include "ProjectConstants.h"

class Synthesizer 
{

public:

	Synthesizer();
	~Synthesizer();

	void init();
	void update();
	void output(float * _output, int _bufferSize, int _nChannels);

	void setVolmue(float _volume);

	Oscillator	m_oscillator[ProjectConstants::NUM_OSCILLATORS];


private:

	float		m_masterVolume;

	ofMutex		m_synthesizerMutex;
};