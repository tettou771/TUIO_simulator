#pragma once

#include "ofMain.h"
#include "ofxTuioServer.h"

class CursorObject {
public:
	CursorObject();
	CursorObject(ofxTuioServer *tuio, ofVec2f pos, ofVec2f vel, ofVec2f acc, float lifeTime);
	~CursorObject();

	void update();

	inline bool isDead() {
		return 
			ofGetElapsedTimef() - beginTime > lifeTime
			|| pos.x < 0 || 1.0 < pos.x
			|| pos.y < 0 || 1.0 < pos.y
			;
	}

	ofxTuioServer *tuio;
	int index;
	ofVec2f pos, vel, acc;
	float beginTime, lifeTime;
	TuioCursor *tuioCursor;

	static unsigned int incrementNum;

private:
	inline double canonicalPosX() {
		return ofMap(pos.x, 0, 1.0, 0, ofGetWidth());
	}
	inline double canonicalPosY() {
		return ofMap(pos.y, 0, 1.0, 0, ofGetHeight());
	}
};

