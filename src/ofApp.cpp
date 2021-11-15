#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("TUIO_simulator");
	ofSetFrameRate(40);

	uiWidth = 230;
	loadConfig();
	tuio.start(tuioAddress, tuioPort);

	string tuioInfo = "TUIO  " + ofToString(tuioAddress) + ":" + ofToString(tuioPort);
	ui = new ofxUISuperCanvas(tuioInfo);
	ui->setWidth(uiWidth - 10);
	ui->addFPS();
	ui->addSlider("cursorLifeTime", 1, 100, &cursorLifeTime);
	ui->addSlider("cursorGenFreq", 0, 1, &cursorGenFreq);
	ui->addIntSlider("cursorNum", 0, 100, &cursorNum);
	ui->autoSizeToFitWidgets();

	// default
	cursorLifeTime = 10;
	cursorGenFreq = 0.1;

	screenAreaReshape();
}

//--------------------------------------------------------------
void ofApp::update(){
	int genNum = ofRandom(0, 1.0) + cursorGenFreq / ofGetFrameRate();
	for (int i = 0; i < genNum; ++i) {
		cursorRandomGenerate();
	}
	cursorNum = cursors.size();

	for (int i = 0; i < cursors.size(); ++i) {
		auto &c = cursors[i];
		if (c->isDead()) {
			delete c;
			cursors.erase(cursors.begin() + i);
		}
		else {
			c->update();
		}
	}

	tuio.run();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(70);

	// draw screen area
	ofDisableAntiAliasing();
	ofSetLineWidth(2);
	ofSetColor(255);
	ofDrawRectangle(screenArea);
	ofSetLineWidth(1);
	ofSetColor(255, 100);
	ofPushMatrix();
	ofTranslate(screenArea.x, screenArea.y);
	int row = 2, col = 2;
	for (int i = 1; i < row; ++i) {
		int x = screenArea.width * i / row;
		ofDrawLine(x, 0, x, screenArea.height);
	}
	for (int i = 1; i < col; ++i) {
		int y = screenArea.height * i / col;
		ofDrawLine(0, y, screenArea.width, y);
	}
	ofSetCircleResolution(100);
	ofDrawCircle(screenArea.width/2, screenArea.height/2, MIN(screenArea.width/2, screenArea.height/2));
	ofPopMatrix();

	// draw cursors
	ofPushMatrix();
	ofTranslate(screenArea.x, screenArea.y);
	ofSetColor(255);
	ofNoFill();

	auto tuioCursors = tuio.tuioServer->getTuioCursors();
	for (auto &c : tuioCursors) {
		ofVec2f pos(c->getPosition().getX() * screenArea.width, c->getPosition().getY() * screenArea.height);
		ofPushMatrix();
		ofTranslate(pos);
		float size = 10;
		ofDrawLine(-size, 0, size, 0);
		ofDrawLine(0, -size, 0, size);
		string txt = "ID:" + ofToString(c->getCursorID());
		txt += "\n(" + ofToString(c->getPosition().getX(), 4) + ", " + ofToString(c->getPosition().getY(), 4) + ")";
		ofDrawBitmapString(txt, 10, 15);
		ofPopMatrix();
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (mouseTuioCursor != nullptr) {
		tuio.updateCursor(mouseTuioCursor, canonicalMousePosX(), canonicalMousePosY());
		cout << "move " << x << ',' << y << endl;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (mouseTuioCursor == nullptr) {
		if (screenArea.inside(x, y)) {
			mouseTuioCursor = tuio.addCursor(canonicalMousePosX(), canonicalMousePosY());
			cout << "add " << x << ',' << y << endl;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if (mouseTuioCursor != nullptr) {
		tuio.removeCursor(mouseTuioCursor);
		mouseTuioCursor = nullptr;
		cout << "remove " << x << ',' << y << endl;
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	screenAreaReshape();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

void ofApp::loadConfig() {
	auto strToChar = [](string & inputStr, char * out) {
		out = new char[inputStr.length() + 1];
		copy(inputStr.begin(), inputStr.end(), out);
		out[inputStr.size()] = '\0';
	};

	ofxXmlSettings xml;
	string configFilename = "config.xml";

	// if not exists, load default
	if (!ofFile(configFilename).isFile()) {
		configFilename = "config-sample.xml";
	}

	xml.loadFile(configFilename);

	if (xml.pushTag("config")) {
		// tuio config
		if (xml.tagExists("tuio")) {
			// TUIO
			tuioAddressStr = xml.getAttribute("tuio", "address", "127.0.0.1", 0);
			//strToChar(tuioAddressStr, tuioAddress);

			tuioAddress = new char[tuioAddressStr.length() + 1];
			copy(tuioAddressStr.begin(), tuioAddressStr.end(), tuioAddress);
			tuioAddress[tuioAddressStr.size()] = '\0';

			tuioPort = xml.getAttribute("tuio", "port", 3333, 0);
		}

		// size
		float w = xml.getAttribute("size_pix", "width", 100, 0);
		float h = xml.getAttribute("size_pix", "height", 100, 0);
		screenAspect = w / h;
		if (isnan(screenAspect) || screenAspect == 0) {
			screenAspect = 1.0;
		}
		screenMargin = 12;
		ofSetWindowShape(w + screenMargin * 2 + uiWidth, h + screenMargin * 2);

		xml.popTag();
	}
}

void ofApp::screenAreaReshape() {
	screenArea.width = (ofGetWidth() - uiWidth) - screenMargin * 2;
	screenArea.height = screenArea.width / screenAspect;
	if (screenArea.height > ofGetWindowHeight() - screenMargin * 2) {
		screenArea.height = ofGetWindowHeight() - screenMargin * 2;
		screenArea.width = screenArea.height * screenAspect;
	}
	screenArea.x = uiWidth + screenMargin;
	screenArea.y = screenMargin;
}

void ofApp::cursorRandomGenerate() {
	auto pos = ofVec2f(ofRandom(0, 1.0), ofRandom(0, 1.0));
	auto vel = ofVec2f(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0)) * 0.1;
	auto acc = ofVec2f(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0)) * 0.1;
	cursorGenerate(pos, vel, acc);
}

void ofApp::cursorGenerate(ofVec2f pos, ofVec2f vel, ofVec2f acc) {
	cursors.push_back(new CursorObject(&tuio, pos, vel, acc, cursorLifeTime));
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
