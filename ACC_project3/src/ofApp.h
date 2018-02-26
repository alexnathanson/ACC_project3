#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		string typing;
		string newString;
		
		void audioOut(float * input, int bufferSize, int nChannels);

		ofSoundStream soundStream;

		vector <float> lAudio;
		vector <float> rAudio;

		float 	pan;
		int		sampleRate;
		bool 	bNoise;
		float 	volume;

		float t; //time

		float readingSpeed;
			
		bool pause;

		//------------------- for the simple sine wave synthesis
		float 	targetFrequency;
		float 	phase;
		float 	phaseAdder;
		float 	phaseAdderTarget;

		float heightPct;

		int bufferSize;

		char asciiVal;

		void stringFrequency(string sortString);
		vector<vector <char> > freqString;
		vector<char> freqChar;


		int checkFrequency(vector<vector <char> > checkVec, char checkChar);
};
