#pragma once

#include "ofMain.h"

class Oscillator 
{

public:
	
	enum WaveType 
	{
		Sine,
		Square,
		Saw,
		ReverseSaw,
		Triangle
	};

	Oscillator();
	~Oscillator();

	void init();
	void update();
	
	void output(std::vector<float> * _output, int _bufferSize, int _nChannels);

	void setPhase(double _phase);
	void setFrequency(float _frequency);
	void setType(WaveType _type);
	void setType(int _type);
	void setVolume(float _volume);
	void setResolution(int _resolution);

	double getPhase();
	float getFrequency();
	WaveType getType();
	float getVolume();
	int getResolution();
	

private:

	std::vector<float> m_waveform; // Oscillator lookup table
	double		m_phase;
	float		m_frequency;
	WaveType	m_type;
	float		m_volume;
	int			m_resolution;

	ofMutex		m_waveformMutex;
};