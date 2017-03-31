#include "ofApp.h"

void ofApp::setup(){
	loadConfig();
	tcpInit();
	streamInit();
	guiInit();
	gamepadInit();
}

void ofApp::update(){
	streamLoop();
	gamepadLoop();
	tcpLoop();
	guiLoop();
}

void ofApp::keyReleased(int key) {
	switch (key) {
		case 'f': {
			ofToggleFullscreen();
			break;
		}
		default: break;
	}
}
