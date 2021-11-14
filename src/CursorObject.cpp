#include "CursorObject.h"

unsigned int CursorObject::incrementNum = 0;

CursorObject::CursorObject() {
}

CursorObject::CursorObject(ofxTuioServer * tuio, ofVec2f pos, ofVec2f vel, ofVec2f acc,float lifeTime)
	: tuio(tuio), pos(pos), vel(vel), acc(acc), lifeTime(lifeTime) {
	beginTime = ofGetElapsedTimef();
	index = ++incrementNum;

	auto list = tuio->tuioServer->getTuioCursors();
	for (int i = 1; i < INT_MAX; ++i) {
		for (auto c : list) {
			if (c->getCursorID() == i) {
				continue;
			}
		}

		//index = i;
		break;
	}

	tuioCursor = tuio->addCursor(canonicalPosX(), canonicalPosY());
}

CursorObject::~CursorObject() {
	tuio->removeCursor(tuioCursor);
}

void CursorObject::update() {
	float fps = ofGetFrameRate();
	if (!isfinite(fps)) fps = 1.0;
	vel += acc / fps;
	pos += vel / fps;

	tuio->updateCursor(tuioCursor, canonicalPosX(), canonicalPosY());
}
