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
		string newStringA;
		string newStringB;

		
		void audioOut(float * input, int bufferSize, int nChannels);
		//void audioOut(ofSoundBuffer &output);

		ofSoundStream soundStream;

		vector <float> lAudio;
		vector <float> rAudio;

		float 	pan;
		int		sampleRate;
		bool 	bNoise;
		float 	volumeOne;

		float t; //time

		float readingSpeed;
			
		bool pause;
		
		int textH;
		int textH2;

		//------------------- for the wave synthesis
		float 	targetFrequencyA;
		float 	phaseA;
		float 	phaseAdderA;
		float 	phaseAdderTargetA;

		float 	targetFrequencyB;
		float 	phaseB;
		float 	phaseAdderB;
		float 	phaseAdderTargetB;

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
		ofxToggle	searchChannel;
		ofxFloatSlider	masterVol;
		ofxIntSlider	start;
		ofxIntSlider	duration;
		ofxFloatSlider	freqA;
		ofxFloatSlider	freqB;
		ofxToggle	volScale;
		ofxFloatSlider	lerpIncrement;
		ofxToggle	muteA;
		ofxToggle	muteB;

		float muteAf;
		float muteBf;

		float volScaler;

		int setMode;

		void searchText(string input);
		void searchTime(string input);

		string formatDate(int month);

		vector <vector <int> > yearMonth;

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

		void prepVec(vector<vector <int> >);
		int vecMaxA;
		int vecMinA;
		int vecItemsA;
		vector<int> spilledVecA;

		int vecMaxB;
		int vecMinB;
		int vecItemsB;
		vector<int> spilledVecB;

		int globalMax;
		int globalMin;

		int hitVal;

		int currentDate;
		int currentVal;

		float sampleA;
		float sampleB;

		float volA;
		float volB;

		float waveSelector(float phase, int waveType);
		int pickWave;
		string thisWave;

		void stringWave();

		void changeSound(bool AB);
		float lerpedVol;
		int lerpPlace;

		float destinationA;
		float prevDestA;
		float lerpAmtA;


		float destinationB;
		float prevDestB;
		float lerpAmtB;

		bool lerpStep;

		void channelMute();

		float positionX;
		float	positionY;
		float positionZ;
};
