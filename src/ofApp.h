#pragma once

#include "ofMain.h"

#include "Synthesizer.h"
#include "Oscillator.h"

#include "ofxImGui.h"


class ofApp : public ofBaseApp{

	public:
		//types
		struct ArduinoInputs 
		{
			int 	forceVal;
			float	distVal;
			ofVec2f	joystickVal;
		};

		struct OscillatorInputs 
		{
			float					frequency;
			float					volume; 
			int	type;
		};
		
		//methods
		void setup();
		void update();
		void draw();

		void drawGUI();
		
		void audioOut(float * _output, int _bufferSize, int _nChannels);

		void setupArduino(const int & _version);
		void updateArduino();

		float getIRDistance(int _value)
		{
			if (_value < 16)
			{
				_value = 16;
			}

			return 2076.0f / (_value - 11.0f);
		}

		//variables
		ofTrueTypeFont		m_font;
		ofxImGui			m_gui;
		ofArduino			m_arduino;

		ArduinoInputs		m_arduinoInputs;

		bool				m_bSetup;
		
		Synthesizer			m_synth;
		OscillatorInputs	m_oscillatorInputs[ProjectConstants::NUM_OSCILLATORS];
		float				m_masterVolume;
};
