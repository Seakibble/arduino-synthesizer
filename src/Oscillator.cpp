#include "Oscillator.h"
#include "ProjectConstants.h"

//--------------------------------------------------------------
Oscillator::Oscillator() {

}
Oscillator::~Oscillator() {

}


//--------------------------------------------------------------
void Oscillator::init() 
{
	m_phase = 0.0f;
	m_type = Sine;
	m_frequency = 300.0f;
	m_volume = 0.0f;
	m_resolution = 32;
}

//--------------------------------------------------------------
void Oscillator::update() 
{
	ofScopedLock waveformLock(m_waveformMutex);

	ofLogNotice("resolution = " + ofToString(m_resolution));
	m_waveform.resize(m_resolution);
	ofLogNotice("size = " + ofToString(m_waveform.size()));

	// "waveformStep" maps a full oscillation of sin() to the size of the waveform lookup table
	float waveformStep = (PI * 2.0) / (float)m_waveform.size();

	for (int i = 0; i < m_waveform.size(); i++) 
	{
		// *** Synth wave types ***
		switch (m_type)
		{
		case Oscillator::Sine:
			m_waveform[i] = sin(i * waveformStep);
			break;

		case Oscillator::Square:
			m_waveform[i] = sin(i * waveformStep);

			if (m_waveform[i] >= 0) {
				m_waveform[i] = 1;
			}
			else {
				m_waveform[i] = -1;
			}
			break;

		case Oscillator::Saw:
			m_waveform[i] = fmod(i * waveformStep, TWO_PI) / PI - 1.0f;
			break;

		case Oscillator::ReverseSaw:
			m_waveform[i] = -(fmod(i * waveformStep, TWO_PI) / PI - 1.0f);
			break;

		case Oscillator::Triangle:
			m_waveform[i] = abs( fmod(i * waveformStep, TWO_PI) / PI - 1.0f );
			break;

		default:
			break;
		}

		m_waveform[i] *= m_volume;
	}


}


//--------------------------------------------------------------
void Oscillator::output(std::vector<float> * _output, int _bufferSize, int _nChannels)
{
	if (m_waveform.size() > 0) {
		ofScopedLock waveformLock(m_waveformMutex);
		float phaseStep = m_frequency / (float)ProjectConstants::SAMPLE_RATE;

		for (int i = 0; i < _bufferSize; i++)
		{
			m_phase += phaseStep;
			int waveformIndex = (int)(m_phase * m_waveform.size()) % m_waveform.size();

			for (int j = 0; j < _nChannels; j++) 
			{
				_output->at(i + j) = m_waveform[waveformIndex];
			}

		}
	}
}



// ACCESSORS ************************************************************


//--------------------------------------------------------------
void Oscillator::setPhase(double _phase)
{
	m_phase = _phase;
}


//--------------------------------------------------------------
void Oscillator::setFrequency(float _frequency)
{
	m_frequency = _frequency;
}


//--------------------------------------------------------------
void Oscillator::setType(WaveType _type)
{
	m_type = _type;
}

void Oscillator::setType(int _type)
{
	switch (_type)
	{
	case 0:
		m_type = Sine;
		break;

	case 1:
		m_type = Square;
		break;

	case 2:
		m_type = Saw;
		break;

	case 3:
		m_type = ReverseSaw;
		break;

	case 4:
		m_type = Triangle;
		break;
	default:
		break;
	}
}


//--------------------------------------------------------------
void Oscillator::setVolume(float _volume)
{
	m_volume = _volume;
}


//--------------------------------------------------------------
void Oscillator::setResolution(int _resolution)
{
	m_resolution = _resolution;
}

//--------------------------------------------------------------
double Oscillator::getPhase()
{
	return m_phase;
}


//--------------------------------------------------------------
float Oscillator::getFrequency()
{
	return m_frequency;
}


//--------------------------------------------------------------
Oscillator::WaveType Oscillator::getType()
{
	return m_type;
}


//--------------------------------------------------------------
float Oscillator::getVolume()
{
	return m_volume;
}


//--------------------------------------------------------------
int Oscillator::getResolution()
{
	return m_resolution;
}