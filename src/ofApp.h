#pragma once

#include "ofMain.h"
#include "ofxTuioServer.h"
#include "ofxXmlSettings.h"
#include "ofxUI.h"
#include "CursorObject.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	// TUIO
	ofxTuioServer tuio;
	string tuioAddressStr;
	char * tuioAddress;
	int tuioPort;

	// TUIO cursor of mouse
	TuioCursor * mouseTuioCursor;

	float screenAspect, screenMargin;
	float uiWidth;

	void loadConfig();

	ofRectangle screenArea;
	void screenAreaReshape();

	vector<CursorObject *> cursors;
	float cursorLifeTime;
	float cursorGenFreq;
	void cursorRandomGenerate();
	int cursorNum;
	int cursorIncrementNum;
	void cursorGenerate(ofVec2f pos, ofVec2f vel, ofVec2f acc);
	ofxUISuperCanvas *ui;

	inline double canonicalMousePosX() {
		return ofMap(ofGetMouseX(), screenArea.x, screenArea.width + screenArea.x, 0, ofGetWidth());
	}
	inline double canonicalMousePosY() {
		return ofMap(ofGetMouseY(), screenArea.y, screenArea.height + screenArea.y, 0, ofGetHeight());
	}

};
