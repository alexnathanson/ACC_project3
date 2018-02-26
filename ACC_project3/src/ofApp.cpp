//ACC Project 3
//Alex Nathanson & Mianying Chen
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);


	bufferSize = 512;
	sampleRate = 44100;
	phase = 0;
	phaseAdder = 0.0f;
	phaseAdderTarget = 0.0f;
	volume = 0.1f;
	bNoise = false;

	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);

	soundStream.printDeviceList();

	soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);

	pause = false;

	readingSpeed = 33; //1000 would be 1 letter per second
	newString = "";
}

//--------------------------------------------------------------
void ofApp::update() {
	ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::draw(){
	//text
	ofSetColor(100, 200, 100);
	ofFill;
	ofDrawRectangle(0, 0, 500, 200);
	ofSetColor(100, 0, 0);
	ofDrawBitmapString(typing, 10, 0);
	ofDrawBitmapString(newString, 10, 20);
	ofDrawBitmapString("Tab = start/ star", 10, 40);
	ofDrawBitmapString("Volume: " + ofToString(volume), 10, 60);
	ofDrawBitmapString("Move the mouse left and right to change the speed", 10, 80);
	ofDrawBitmapString("Reading speed: " + ofToString(readingSpeed), 10, 100);
	ofDrawBitmapString("Type something and press enter", 10, 120);



}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//uncomment out if you want to see the ascii value of something
	//std::cout << key << "\n";

	if (key == OF_KEY_RETURN) {
		newString = typing;
		typing = "";
		//stringFrequency(newString);
	} else if (key == OF_KEY_BACKSPACE) {
		if (typing != "") {
			typing.erase(typing.length() - 1, typing.length());
		}
	} else if (key == '-' || key == '_') {
		volume -= 0.05;
		volume = MAX(volume, 0);
	} else if (key == '+' || key == '=') {
		volume += 0.05;
		volume = MIN(volume, 1);
	} else if (key == 9) {
		if (pause) {
			soundStream.start();
			pause = false;
		} else {
			soundStream.stop();
			pause = true;
		}
	} else {
		typing += key;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	readingSpeed = (float)ofMap(x, 0, ofGetWidth(), 10, 1000);

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


/*
audioOut is an asynchronous function that runs outside our main draw/update loops
output is always an array of samples. We can represent arrays using pointers
*/
//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
	//pan = 0.5f;
	float leftScale = 1 - pan;
	float rightScale = pan;


	////pan based on position of word
	//if (newString != "") {
	//	for (int w = 0; w < newString.length(); w++) {
	//		pan = (float)w / (float)newString.length();

	//		asciiVal = newString.at(w);

	//		std::cout << newString.at(w) << "\n";

	//		targetFrequency = 2000.0f * (int)asciiVal;
	//		phaseAdderTarget = (targetFrequency / (float)sampleRate) * TWO_PI;
	//	}
	//}

	//buffer chunk
	float buffChunk = bufferSize / (float)newString.length();

	// sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-TWO_PI like this:
	

	//phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
		
	if (newString.length() != 0) {
			
		t = fmod(ofGetElapsedTimeMillis(),(float)newString.length() * readingSpeed);
		//map the buffer position to the string
		int mapString = (int)ofMap(t, 0.0, (float)newString.length() * readingSpeed, 0.0, (float)newString.length());

		asciiVal = newString.at(mapString);
		//std::cout << "T: "<<t <<" MAP: "<<mapString<< "\n";

		phaseAdderTarget = ((2000.0f * (float)asciiVal) / (float)sampleRate) * TWO_PI;


	}
		
	phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;

	while (phase > TWO_PI) {
		phase -= TWO_PI;
	}

	for (int i = 0; i < bufferSize; i++) {

			

		//phase = ;
		//phase = (targetFrequency / (float)sampleRate) * TWO_PI;
		//std::cout << "Phase: "<<phase << "\n";
		//not sure why the example used a while loop rather than an if statement here
			
		phase += phaseAdder;

		float sample = sin(phase);
		lAudio[i] = output[i*nChannels] = sample * volume * leftScale;
		rAudio[i] = output[i*nChannels + 1] = sample * volume * rightScale;
	}
}

void ofApp::stringFrequency(string sortString) {

	freqString.clear();

	if (sortString != "") {
		for (int i = 0; i < sortString.length(); i++) {
			freqChar.clear();
			
			//check if char is already in string
			for (int b = 0; b < freqString.size(); b++) {
				if (sortString.at(i) == freqString[b][0]) {
					freqString[i][1] ++;
					break;
				}
				else {
					freqChar.push_back(sortString.at(i));
					freqChar.push_back(1);
					freqString.push_back(freqChar);
				}
			}
		}
		std::cout << "Unique characters: " << freqString.size() << "\n";
	}
}