#include "ofApp.h"
#include "ProjectConstants.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(ProjectConstants::PROJ_TARGET_FPS);

	m_font.load("franklinGothic.otf", 16);

	// replace the string below with the serial port for your Arduino board
	// you can get this from the Arduino application (Tools Menu -> Port) 
	m_arduino.connect("COM3", 57600);

	m_arduinoInputs.forceVal = 0;
	m_arduinoInputs.distVal = 0;
	m_arduinoInputs.joystickVal = ofVec2f(0.0f, 0.0f);

	// Listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	// call setupArduino()
	ofAddListener(m_arduino.EInitialized, this, &ofApp::setupArduino);


	m_masterVolume = 0.0f;

	for (int i = 0; i < ProjectConstants::NUM_OSCILLATORS; i++)
	{
		m_oscillatorInputs[i].frequency = 50.0f;
		m_oscillatorInputs[i].volume = 1.0f;
		m_oscillatorInputs[i].type = 0;
	}

	m_bSetup = false;
}

//--------------------------------------------------------------
void ofApp::setupArduino(const int & _version)
{
	// remove listener because we don't need it anymore
	ofRemoveListener(m_arduino.EInitialized, this, &ofApp::setupArduino);

	// print firmware name and version to the console
	ofLogNotice() << m_arduino.getFirmwareName();
	ofLogNotice() << "firmata v" << m_arduino.getMajorFirmwareVersion() << "." << m_arduino.getMinorFirmwareVersion();

	/*  Pin Configuration - configure your pins here
	Our example has:
	Analog Input   - 1 potentiometer on Analog pin 0 (A0)
	Digital Input  - 1 button Digital pin 2 (D2)
	Digital Output - 1 LED Digital pin 4 (D4)

	Digital PWM Output - 1 LED on Digital pin 11 (D11)
	*/

	// set pin A0 to analog input
	m_arduino.sendAnalogPinReporting(ProjectConstants::FORCE_PIN, ARD_ANALOG);
	m_arduino.sendAnalogPinReporting(ProjectConstants::DIST_PIN, ARD_ANALOG);

	m_arduino.sendAnalogPinReporting(ProjectConstants::JOYSTICK_X_PIN, ARD_ANALOG);
	m_arduino.sendAnalogPinReporting(ProjectConstants::JOYSTICK_Y_PIN, ARD_ANALOG);

	m_bSetup = true;
}

//--------------------------------------------------------------
void ofApp::update()
{
	// Sometimes the EInitialized event does not fire reliably for Arduino.
	// As a failsafe, if the arduino is not set up after 5 seconds, force it
	// to be set up.
	if (!m_bSetup && ofGetElapsedTimeMillis() > 5000)
	{
		setupArduino(0);
	}

	if (m_bSetup) {
		updateArduino();

		if (ofGetKeyPressed()) {
			m_masterVolume = ofLerp(m_masterVolume, 0.0f, 0.8f);
		}
		else {
			m_masterVolume = ofLerp(m_masterVolume, 0.0f, 0.1f);
		}

		m_synth.setVolmue(m_masterVolume);
		
		for (int i = 0; i < ProjectConstants::NUM_OSCILLATORS; i++)
		{
			m_synth.m_oscillator[i].setFrequency(m_oscillatorInputs[i].frequency);
			m_synth.m_oscillator[i].setVolume(m_oscillatorInputs[i].volume);
			m_synth.m_oscillator[i].setType(m_oscillatorInputs[i].type);
		}

		m_synth.update();

	}
}

//--------------------------------------------------------------
void ofApp::updateArduino() {

	// update the arduino, get any data or messages.
	// the call to m_arduino.update() is required
	m_arduino.update();

	m_arduinoInputs.forceVal = m_arduino.getAnalog(ProjectConstants::FORCE_PIN);
	m_arduinoInputs.distVal = getIRDistance(m_arduino.getAnalog(ProjectConstants::DIST_PIN));

	m_arduinoInputs.joystickVal = ofVec2f(
		m_arduino.getAnalog(ProjectConstants::JOYSTICK_X_PIN),
		m_arduino.getAnalog(ProjectConstants::JOYSTICK_Y_PIN)
		);

}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(100, 0, 130);

	ofEnableAlphaBlending();
	ofSetColor(0, 0, 0, 127);
	ofDrawRectangle(510, 15, 275, 150);
	ofDisableAlphaBlending();

	ofSetColor(140, 140, 255);

	// Draw sensor input values
	m_font.drawString("Force Value: " + ofToString(m_arduinoInputs.forceVal), 530, 105);
	m_font.drawString("Dist Value: " + ofToString(m_arduinoInputs.distVal), 530, 135);

	m_font.drawString("Joystick Value: (" + ofToString(m_arduinoInputs.joystickVal.x) + ", " + ofToString(m_arduinoInputs.joystickVal.y) + ")", 530, 155);
	// remap our force values (can check on arduino sketch as values will always vary between sensors)
	float radius = ofMap(m_arduinoInputs.forceVal, 0, 1024, 20, 150);
	ofDrawCircle(200, 400, radius);

	radius = ofMap(m_arduinoInputs.distVal, 4.0f, 30.0f, 20.0f, 150.0f, true);
	ofDrawCircle(400, 400, radius);

	ofPushMatrix();
		ofDrawCircle(600 + (m_arduinoInputs.joystickVal.x / 10.0f), 400 + (m_arduinoInputs.joystickVal.y / 10.0f), 20);
	ofPopMatrix();

	drawGUI();
}


//--------------------------------------------------------------
void ofApp::drawGUI()
{
	ofFill();
	m_gui.begin();

	ImGui::SliderFloat("Master Volume", &m_masterVolume, 0.0f, 1.0f);

	for (int i = 0; i < ProjectConstants::NUM_OSCILLATORS; i++)
	{
		ImGui::PushID(i);
		ImGui::Text("Oscillator %d", ofToString(i + 1));
		
		ImGui::SliderFloat("Volume", &m_oscillatorInputs[i].volume, 0.0f, 1.0f);
		ImGui::SliderFloat("Fequency", &m_oscillatorInputs[i].frequency, 40.0f, 3500.0f);

		ImGui::RadioButton("Sine", &m_oscillatorInputs[i].type, 0); ImGui::SameLine();
		ImGui::RadioButton("Square", &m_oscillatorInputs[i].type, 1); ImGui::SameLine();
		ImGui::RadioButton("Saw", &m_oscillatorInputs[i].type, 2); ImGui::SameLine();
		ImGui::RadioButton("Reverse Saw", &m_oscillatorInputs[i].type, 3); ImGui::SameLine();
		ImGui::RadioButton("Triangle", &m_oscillatorInputs[i].type, 4);


		ImGui::PopID();
	}

	m_gui.end();
}


//--------------------------------------------------------------
void ofApp::audioOut(float * _output, int _bufferSize, int _nChannels)
{
	m_synth.output(_output, _bufferSize, _nChannels);
}