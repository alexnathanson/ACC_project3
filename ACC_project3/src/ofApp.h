#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxGui.h"
#include "jsonThread.h"

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
		
		int textH;
		int textH2;

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

		//not currently implemented
		vector<string> freqWord;
		vector<vector <string> > freqAllStrings;

		int checkFrequency(vector<vector <char> > checkVec, char checkChar);

		string playString;

		//NYT API
		string url;
		string searchKey;
		string mString;

		ofxJSONElement json;

		bool newSearch;
		bool parsingSuccessful;

		int line;

		void newLine(string jsonString);

		//GUI
		ofxPanel gui;

		void setupGui();
		ofxToggle	mode;
		ofxToggle	loop;

		int setMode;

		void searchText(string input);
		void searchTime(string input);


		void playText(string pTe);
		void playTime(string pTi);

		string formatDate(int month);

		vector <vector <int> > yearMonth;

		//mutex
		//mutex getJson;

		jsonThread getJSON;

		bool loading;

		bool newTime;

		int yStart;
		int years;
		int y;
		int m;
		bool allDone;

		string year;
		string month;
		string end;
		string begin;

		vector <int> hitMonth;

		//Json::Value aHit;
		int aHit;
		void printVec(vector<vector <int> >);
};
