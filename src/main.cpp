#include "ofMain.h"
#include "ofApp.h"
#include "ProjectConstants.h"

//========================================================================
int main( ){
	ofSetupOpenGL(ProjectConstants::PROJ_SCREEN_RES_X, ProjectConstants::PROJ_SCREEN_RES_Y,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
