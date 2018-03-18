//ACC Project 3
//Alex Nathanson & Mianying Chen
#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);

	setupGui();

	setMode = 0;

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
	playString = "";

	textH = 0;
	textH2 = 500;

	newSearch = false;

	line = 0;

	searchKey = "8c66b14d868d480b89812149a2288372";

	loading = false;
	newTime = false;
	allDone = true;

}

//--------------------------------------------------------------
void ofApp::update() {
	ofBackground(0);

	//seperate by word and add + signs
	vector <string> format = ofSplitString(newString, " "); //doesn't need to be cleared because it's being initialized here
	
	string formed;
	formed = "";

	for (int r = 0; r < format.size(); r++) {
		formed = format[r] + "+";
	}

	formed.erase(formed.size(), 1); //erase the last +

	if (newSearch == true) {
	std:cout << "mode: " << mode << endl;
		switch (mode) {
		case 0:
			searchText(formed);
			break;
		case 1:
			searchTime(formed);
			break;
		}
	}

	if (!allDone) {
		searchTime(formed);
	}
	
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	//text
	ofSetColor(100, 200, 100);
	ofFill;
	ofDrawRectangle(0, textH, 500, 180);
	ofSetColor(100, 0, 0);
	ofDrawBitmapString("New York Times Headline Sonifier", 10, textH + 20);
	ofDrawBitmapString("New search: " +typing, 10, textH + 40);
	ofDrawBitmapString("Current search: " + newString, 10, textH + 60);
	ofDrawBitmapString("Current volume: " + ofToString(volume), 10, textH + 80);
	ofDrawBitmapString("Current Char: " + ofToString(asciiVal), 10, textH + 100);
	ofDrawBitmapString("Reading speed: " + ofToString(readingSpeed), 10, textH + 120);
	ofDrawBitmapString("Unique chars: " + ofToString(freqString.size()), 10, textH + 140);
	ofDrawBitmapString("Current line: " + ofToString(line), 10, textH + 160);

	//ofDrawBitmapString("Unique words: " + ofToString(wordString.size()), 10, textH + 140);



	ofSetColor(100, 200, 100);
	ofFill;
	ofDrawRectangle(450, textH2, ofGetWidth()- 460, 120);
	ofSetColor(100, 0, 0);
	ofDrawBitmapString("Type something to search and press enter", 460, textH2 + 20);
	ofDrawBitmapString("Move the mouse left and right to change the speed", 460, textH2 + 40);
	ofDrawBitmapString("Tab = start/ star", 460, textH2 + 60);
	ofDrawBitmapString("The ascii value of each letter is used to control it's frequency.", 460, textH2 + 80);
	ofDrawBitmapString("The volume is determined by the quantity of a given letter.", 460, textH2 + 100);




	for (Json::ArrayIndex i = 0; i < json["response"]["docs"].size(); ++i)
	{
		std::string title = json["response"]["docs"][i]["headline"]["main"].asString();
		std::string author = json["response"]["docs"][i]["byline"]["original"].asString();
		std::string date = json["response"]["docs"][i]["pub_date"].asString();
		std::string text = title + " - " + author + " (" + date + ")";
		if (i == line) {
			ofSetColor(255, 255, 255);
		}
		else {
			ofSetColor(255, 0, 0);
		}
		ofDrawBitmapString(text, 20, 200 + (i * 20));
	}

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//uncomment out if you want to see the ascii value of something
	//std::cout << key << "\n";

	string specials = "~!@#%^&*()[]<>";

	if (key == OF_KEY_RETURN) {
		newString = typing;
		typing = "";
		newSearch = true;
	} else if (key == OF_KEY_BACKSPACE) {
		if (typing != "") {
			typing.erase(typing.length() - 1, typing.length());
		}
	} else if (key == '-' || key == '_') {
		//volume -= 0.05;
		//volume = MAX(volume, 0);
	} else if (key == '+' || key == '=') {
		//volume += 0.05;
		//volume = MIN(volume, 1);
	} else if (key == 9) {
		if (pause) {
			soundStream.start();
			pause = false;
		} else {
			soundStream.stop();
			pause = true;
		}
	}	else if (key == OF_KEY_UP) {
		if (line < 10) {
			line++;
			newLine(json["response"]["docs"][line]["headline"]["main"].asString());
		}
	} else if(key == OF_KEY_DOWN){
		if (line > 0) {
			line = line - 1;
			newLine(json["response"]["docs"][line]["headline"]["main"].asString());
		}
	} else if (key < 1000 && specials.find(key) == string::npos) {
		//remove special characters
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
	pan = 0.5f;
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
	float buffChunk = bufferSize / (float)playString.length();

	// sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-TWO_PI like this:
	

	//phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
		
	if (playString.length() != 0) {
			
		t = fmod(ofGetElapsedTimeMillis(),(float)playString.length() * readingSpeed);
		//map the buffer position to the string
		int mapString = (int)ofMap(t, 0.0, (float)playString.length() * readingSpeed, 0.0, (float)playString.length());

		volume = (float)checkFrequency(freqString, playString.at(mapString))/ (float)playString.size();


		asciiVal = playString.at(mapString);
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

	bool match;

	if (sortString != "") {
		for (int i = 0; i < sortString.length(); i++) {
			freqChar.clear();
			
			match = 0;

			//check if char is already in string

			if (freqString.size() > 0) {
				for (int b = 0; b < freqString.size(); b++) {
					if (sortString.at(i) == freqString[b][0]) {
						freqString[b][1] ++;
						match = 1;
						break;
					}
				}
			} 
			
			if (match == 0) {
				freqChar.push_back(sortString.at(i));
				freqChar.push_back(1);
				freqString.push_back(freqChar);
			}	
		}
		//std::cout << "Unique characters: " << freqString.size() << "\n";
	}
}

int ofApp::checkFrequency(vector<vector <char> > checkVec, char checkChar) {

	for (int v = 0; v < checkVec.size(); v++) {
		if (checkChar == checkVec[v][0]) {
			return checkVec[v][1];
		}
	}
}

void ofApp::newLine(string jsonString) {
	playString = json["response"]["docs"][line]["headline"]["main"].asString();
	stringFrequency(playString);
}

void ofApp::setupGui() {
	gui.setup();
	gui.add(mode.setup("Mode", true));
	gui.add(loop.setup("Loop ", true));

}

void ofApp::searchText(string input) {

	//old key: 52697f63c9ade478ec6f2c7d71811aa6:17:61363877
	url = "http://api.nytimes.com/svc/search/v2/articlesearch.json?q=" + input + "&page=2&sort=newest&api-key=" + searchKey;

	if (newSearch == true) {
		// Now parse the JSON
		parsingSuccessful = json.open(url);

		newLine(json["response"]["docs"][line]["headline"]["main"].asString());
		/*playString = json["response"]["docs"][line]["headline"]["main"].asString();
		stringFrequency(playString);*/

		newSearch = false;


		if (parsingSuccessful)
		{
			ofLogNotice("ofApp::setup") << json.getRawString(true);
		}
		else {
			ofLogNotice("ofApp::setup") << "Failed to parse JSON.";
		}
	}
}

void ofApp::searchTime(string input) {

	//filter the results
	string timeFil = "";
	/*if (filTog == true) {
		timeFil = ""&filter=(Per)";
	}*/	

	//initialize
	if (newSearch == true) {
		newSearch = false;
		allDone = false;
		y = 2014;
		years = 3;
		yStart = y;

		yearMonth.clear();

		m = 1;

		//restrict to 2017
		//change this to get todays date
		if (y + years > 2017) {
			years = 2017 - y;
		}

		ofLogNotice("Search Time") << "It's time!";
	}

	//check if we're finished
	if (!allDone) {

		if (y > (years + yStart)-1) {
			allDone = true;
			//ofLogNotice("Done", ofToString(y));
		}
		
		//clear the month vector
		if (m == 1) {
			hitMonth.clear();
		}

		//make the next request
		if (!getJSON.isThreadRunning() && loading == false) {

			year = ofToString(y);
			month = formatDate(m);
			end = year + month;
			begin = year + month.replace(2, 2, "01");
						
			loading = true;

			getJSON.setup(input, begin, end, searchKey);
			getJSON.startThread();
		}

		//get data and do something with it
		if (!getJSON.isThreadRunning() && loading == true) {
			if (getJSON.parsingT)
			{
				aHit = getJSON.hits;
				//ofLogNotice("ofApp::setup") << getJSON.jsonT.getRawString(true);
				ofLogNotice("Returned Hits", begin + " " + ofToString(aHit));
			}
			else {
				ofLogNotice("ofApp::setup") << "Failed to parse JSON.";
			}

			loading = false;

			//increment the months + year
			if (m < 12) {
				m++;
				hitMonth.push_back(aHit);
			}
			else {
				hitMonth.push_back(aHit);
				yearMonth.push_back(hitMonth);
				m = 1;
				y++;
			}
		}

		if (allDone) {
			printVec(yearMonth);
		}
	}
}

void ofApp::playText(string pTe) {

}

void ofApp::playTime(string pTi) {

}

string ofApp::formatDate(int month) {

	switch (month) {
		case 1:
			mString = "0131";
			break;
		case 2:
			mString = "0228";
			break;
		case 3:
			mString = "0331";
			break;
		case 4:
			mString = "0430";
			break;
		case 5:
			mString = "0531";
			break;
		case 6:
			mString = "0630";
			break;
		case 7:
			mString = "0731";
			break;
		case 8:
			mString = "0831";
			break;
		case 9:
			mString = "0930";
			break;
		case 10:
			mString = "1031";
			break;
		case 11:
			mString = "1130";
			break;
		case 12:
			mString = "1231";
			break;
	}

	return mString;
}

void ofApp::printVec(vector<vector <int> > vov) {

	for (int a = 0; a < vov.size(); a++) {
		for (int b = 0; b < vov[a].size(); b++) {
			std::cout << vov[a][b] << endl;
		}
	}
}