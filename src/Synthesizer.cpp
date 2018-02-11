#include "Synthesizer.h"
#include "ofMain.h"

//--------------------------------------------------------------
Synthesizer::Synthesizer() {

}

Synthesizer::~Synthesizer() {

}


//--------------------------------------------------------------
void Synthesizer::init() 
{
	m_masterVolume = 1.0f;

	for (int i = 0; i < ProjectConstants::NUM_OSCILLATORS; i++) 
	{
		m_oscillator[i].init();
	}
}


//--------------------------------------------------------------
void Synthesizer::update() 
{

	for (int i = 0; i < ProjectConstants::NUM_OSCILLATORS; i++)
	{
		m_oscillator[i].update();
	}
}


//--------------------------------------------------------------
void Synthesizer::output(float * _output, int _bufferSize, int _nChannels)
{
	ofScopedLock waveformLock(m_synthesizerMutex);

	//create a list of samples for each oscillator
	std::vector<std::vector<float>> m_oscWaveforms;
	m_oscWaveforms.resize(ProjectConstants::NUM_OSCILLATORS);

	//set each list to be the correct size
	for (int i = 0; i < ProjectConstants::NUM_OSCILLATORS; i++)
	{
		m_oscWaveforms[i].resize(_bufferSize * _nChannels);
	}

	//populate lists with each oscillator's waveform
	for (int i = 0; i < ProjectConstants::NUM_OSCILLATORS; i++)
	{
		m_oscillator[i].output(&m_oscWaveforms[i], _bufferSize, _nChannels);
	}

	//cerate a list to contain the final averaged waveform
	std::vector<float> m_averagedWaveform;
	m_averagedWaveform.resize(_bufferSize * _nChannels);

	//	combine samples from each oscillator and average them, weighted by volume
	for (int i = 0; i < _bufferSize * _nChannels; i++){
		float weightedVolume = 0.0f;
		for (int j = 0; j < ProjectConstants::NUM_OSCILLATORS; j++)
		{
			m_averagedWaveform[i] += m_oscWaveforms[j][i];
			weightedVolume += m_oscillator[j].getVolume();
		}
		_output[i] = m_averagedWaveform[i] / weightedVolume * m_masterVolume;
	}
	
}


//--------------------------------------------------------------
void Synthesizer::setVolmue(float _volume)
{
	m_masterVolume = _volume;
}