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
	phaseA = 0;
	phaseAdderA = 0.0f;
	phaseAdderTargetA = 0.0f;
	phaseB = 0;
	phaseAdderB = 0.0f;
	phaseAdderTargetB = 0.0f;
	bNoise = false;

	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);

	soundStream.printDeviceList();
	soundStream.setup(this, 2, 0, sampleRate, bufferSize, 2);

	//ofSoundStreamSetup(2, 0, sampleRate, 512, 2);

	pause = false;

	readingSpeed = 33; //1000 would be 1 letter per second
	newString = "";
	playString = "";

	textH = 0;
	textH2 = 500;

	newSearch = false;

	line = 0;

	//alexnathanson.com key
	//searchKey = "8c66b14d868d480b89812149a2288372";

	//gmail key
	searchKey = "0f9a222e87344442aba1dca7467d70c7";

	loading = false;
	newTime = false;
	allDone = true;

	pickWave = 1;
	thisWave = "sine";

	lerpPlace = 0;
	prevDestA = 0.0;
	prevDestB = 0.0;

	lerpStep = false;

	vecMaxA = 0;
	vecMinA = 10000;
	vecItemsA = 0;
	vecMaxB = 0;
	vecMinB = 10000;
	vecItemsB = 0;
}

//--------------------------------------------------------------
void ofApp::update() {
	ofBackground(0);

	//seperate by word and replace spaces with + signs
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

	if (spilledVecA.size() != 0) {
		changeSound(false);
	}

	if (spilledVecB.size() != 0) {
		changeSound(true);
	}

	channelMute();
}

//--------------------------------------------------------------
void ofApp::draw(){
	//text
	ofSetColor(100, 200, 100);
	ofNoFill;
	ofDrawRectangle(0, textH, 500, 200);
	ofDrawRectangle(450, textH2, ofGetWidth() - 450, ofGetHeight()-textH2);

	ofSetColor(255, 255, 255);
	ofDrawBitmapString("New York Times History Sonifier/ Public Event Sequencer", 10, textH + 20);
	ofDrawBitmapString("New search: " +typing, 10, textH + 40);
	ofDrawBitmapString("Current search A: " + newStringA + " B : " + newStringB, 10, textH + 60);

	//print mode 0 info
	if (mode == 0) {
		ofDrawBitmapString("Current Char: " + ofToString(asciiVal), 10, textH + 80);
		ofDrawBitmapString("Reading speed: " + ofToString(readingSpeed), 10, textH + 100);
		ofDrawBitmapString("Unique chars: " + ofToString(freqString.size()), 10, textH + 120);
		ofDrawBitmapString("Current line: " + ofToString(line), 10, textH + 140);

		ofSetColor(255, 255, 255);
		ofDrawBitmapString("Mode 0: ASCII SOUNDS", 460, textH2 - 5);
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
				ofSetColor(0, 200, 0);
			}
			ofDrawBitmapString(text, 20, 240 + (i * 20));
		}
	}
	else if (mode == 1) { //print mode 1 info
		ofSetColor(255, 255, 255);
		ofDrawBitmapString("Current Date: " + currentDate , 10, textH + 80);
		ofDrawBitmapString("Current Vol A: " + ofToString(volA), 10, textH + 100);
		ofDrawBitmapString("Current Vol B: " + ofToString(volB), 10, textH + 120);
		ofDrawBitmapString("Total hits, current hit, destination: " + ofToString(vecItemsA) + ":" + ofToString(lerpPlace) + ":" + ofToString(destinationA), 10, textH + 140);
		ofDrawBitmapString("Total hits, current hit, destination: " + ofToString(vecItemsB) + ":" + ofToString(lerpPlace) + ":" + ofToString(destinationB), 10, textH + 160);
		ofDrawBitmapString("Wave type: " + thisWave, 10, textH + 180);

		ofSetColor(255, 255, 255);
		ofDrawBitmapString("Mode 1: OCCURANCES OVER TIME", 460, textH2 + 20);

		if (spilledVecA.size() != 0) {
			ofDrawBitmapString("Min: " + ofToString(globalMin) + " Max: " + ofToString(globalMax), 460, textH2 + 40);
			for (int i = 0; i < spilledVecA.size(); i++)
			{
				ofDrawBitmapString(spilledVecA[i], 460, textH2 + 60 + (i * 20));
			}
		}
		if (spilledVecB.size() != 0) {
			for (int i = 0; i < spilledVecB.size(); i++)
			{
				ofDrawBitmapString(spilledVecB[i], 500, textH2 + 60 + (i * 20));
			}
		}
	}

	gui.setPosition(ofGetWidth()*.5, 20);
	gui.draw();

	/*
	MODE 0
	X = ascii value
	Y = 
	Z =
	color = 
	MODE 1
	X = lerp amount
	Y = lerp pos
	Z = freq
	brighter color = higher volume
	shape equals frequencies
	*/
	if (mode) {
		ofSetColor(0, (int)ofMap(volB, 0.0, 1.0, 100, 255), (int)ofMap(volA, 0.0, 1.0, 100, 255));
	}
	else {
		ofSetColor(255, (int)ofMap(volumeOne, 0.0, 1.0, 100, 255), (int)ofMap(volumeOne, 0.0, 1.0, 100, 255));
	}
	float x, y, z;
	float xt, yt, zt;

	float time = ofGetElapsedTimef();
	for (int i = 0; i < 50; i++) {
		float width = ofGetWidth();
		float height = ofGetHeight();
		float speed = 0.55;
		float posX = i * positionX;
		float posY = i * ofMap(lerpPlace, 0, spilledVecA.size(), 0, 530);
		float posZ = i * ofMap(freqA, 0, 2500, 10, 650);
		x = width * ofNoise(time * speed - posX);
		y = height * ofNoise(time * speed + posY);
		z = 90 * ofNoise(time * speed + posZ);
		ofNoFill();
		ofRotateX(x / 400);
		ofRotateY(sin(y / 400));
		ofRotateZ(sin(z / 400));
		int posa = ofMap(freqA, 0, 2500, 10, 60);
		int posb = ofMap(freqB, 0, 2500, 10, 60);
		ofEllipse(x, y, z, posa, posb);
	}
	if (mode) {
		ofSetColor((int)ofMap(volA, 0.0, 1.0, 100, 255), 0, (int)ofMap(volB, 0.0, 1.0, 100, 255));

		for (int i = 0; i < 50; i++) {
			float width = ofGetWidth();
			float height = ofGetHeight();
			float speed = 0.55;
			float posXt = i * positionXt;
			float posYt = i * ofMap(lerpPlace, 0, spilledVecA.size(), 0, 530);
			float posZt = i * ofMap(freqB, 0, 2500, 10, 650);
			xt = width * ofNoise(time * speed - posXt);
			yt = height * ofNoise(time * speed + posYt);
			zt = 90 * ofNoise(time * speed + posZt);
			ofNoFill();
			ofRotateX(x / 400);
			ofRotateY(sin(y / 400));
			ofRotateZ(sin(z / 400));
			int offsetx = ofMap(freqA, 0, 2500, 4, 60);
			int offsety = ofMap(freqB, 0, 2500, 4, 60);
			ofTriangle(xt, yt, zt, xt + offsetx, yt - offsety, zt, xt - offsetx, yt - offsety, zt);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//uncomment out if you want to see the ascii value of something
	//std::cout << key << "\n";

	//a string containing special characters that may or may not break the search API
	string specials = "~!@#%^&*()[]<>";

	if (key == OF_KEY_RETURN) {
		newString = typing;
		if (!searchChannel) {
			newStringA = typing;
		}
		else {
			newStringB = typing;
		}
		typing = "";
		newSearch = true;
	} else if (key == OF_KEY_BACKSPACE) {
		if (typing != "") {
			typing.erase(typing.length() - 1, typing.length());
		}
	} else if (key == '-' || key == '_') {
		pickWave -= 1;
		if (pickWave == 0) {
			pickWave = 4;
		}
		stringWave();
	} else if (key == '+' || key == '=') {
		pickWave += 1;
		if (pickWave == 5) {
			pickWave = 1;
		}
		stringWave();
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
//void ofApp::audioOut(ofSoundBuffer &output) {

	pan = 0.5f;
	float leftScale = 1 - pan;
	float rightScale = pan;

	if (mode == 0) {

		float buffChunk = bufferSize / (float)playString.length();

		if (playString.length() != 0) {

			t = fmod(ofGetElapsedTimeMillis(), (float)playString.length() * readingSpeed);
			//map the buffer position to the string
			int mapString = (int)ofMap(t, 0.0, (float)playString.length() * readingSpeed, 0.0, (float)playString.length());

			volumeOne = (float)checkFrequency(freqString, playString.at(mapString)) / (float)playString.size();

			asciiVal = playString.at(mapString);
			//std::cout << "T: "<<t <<" MAP: "<<mapString<< "\n";
			
			positionX = ofMap(asciiVal, 0, 100, 0.0, 530.0);
			phaseAdderTargetA = ((2000.0f * (float)asciiVal) / (float)sampleRate) * TWO_PI;
		}

		phaseAdderA = 0.95f * phaseAdderA + 0.05f * phaseAdderTargetA;

		// sin (n) seems to have trouble when n is very large, so we
		// keep phase in the range of 0-TWO_PI like this:
		while (phaseA > TWO_PI) {
			phaseA -= TWO_PI;
		}

		for (int i = 0; i < bufferSize; i++) {

			phaseA += phaseAdderA;

			float sample = sin(phaseA);
			lAudio[i] = output[i] = sample * volumeOne * masterVol;
			rAudio[i] = output[i + 1] = sample * volumeOne * masterVol;
		}
	}
	else if (mode == 1) {

		if (spilledVecA.size() != 0) {
			for (int i = 0; i < bufferSize; i += 2) {
				//phase = ((float)freqA / (float)sampleRate);
				float sampleA = waveSelector(phaseA, pickWave) * (float)volA  * muteAf; // generating a sine wave sample
				float sampleB = waveSelector(phaseB, pickWave) * (float)volB * muteBf;
				float sample = (sampleA + sampleB) * .5;
				output[i] = sample * masterVol; // writing to the left channel
				output[i + 1] = sample * masterVol; // writing to the right channel
				//memorize this equation! phaseOffset (angle) = freq / sampleRate
				float phaseOffsetA = ((float)freqA / (float)sampleRate);
				phaseA += phaseOffsetA;
				float phaseOffsetB = ((float)freqB / (float)sampleRate);
				phaseB += phaseOffsetB;
			}
		}
	}
}

//determing and store the frequency of a given char in Mode 0
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

//check the frequency of a given char in mode 0
int ofApp::checkFrequency(vector<vector <char> > checkVec, char checkChar) {

	for (int v = 0; v < checkVec.size(); v++) {
		if (checkChar == checkVec[v][0]) {
			return checkVec[v][1];
		}
	}
}

//set which line is being played in mode 0
void ofApp::newLine(string jsonString) {
	playString = json["response"]["docs"][line]["headline"]["main"].asString();
	stringFrequency(playString);
}

void ofApp::setupGui() {
	gui.setup();
	gui.add(mode.setup("Mode", true));
	gui.add(searchChannel .setup("Search Channel", false));
	gui.add(masterVol.setup("Master Volume", .5, 0.0, 1.0));
	gui.add(start.setup("Start Year", 2016, 1960, 2017));
	gui.add(duration.setup("Search Duration", 1, 1, 10));
	gui.add(freqA.setup("frequency A", 200.0f, 60.0f, 2500.0f));
	gui.add(freqB.setup("frequency B", 800.0f, 60.0f, 2500.0f));
	gui.add(volScale.setup("Volume Scale", false));
	gui.add(lerpIncrement.setup("Lerp Increment", 30.0, 0.5, 60.0));
	gui.add(muteA.setup("Mute A", false));
	gui.add(muteB.setup("Mute B", false));
}

//Mode 0 search API
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

//Mode 1 search API
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
		loading = false;
		y = start;
		ofLogNotice() << "new search!" << endl;
		years = duration;
		yStart = y;

		yearMonth.clear();

		m = 1;

		//restrict to 2017
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
			//ofLogNotice("begin", year + " " + month + " = " + begin);

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
				if (!allDone) {
					y++;
				}
			}
			//if (allDone) {//do I need an else statement here?
			//	y = start;
			//	ofLogNotice("restarted?", "yes!");
			//}

		}

		if (allDone) {
			//y = start;
			printVec(yearMonth);
		}
	}
}

//formated the date as a string for the search
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

//print the hits returned by the search 
void ofApp::printVec(vector<vector <int> > vov) {

	for (int a = 0; a < vov.size(); a++) {
		for (int b = 0; b < vov[a].size(); b++) {
			std::cout << vov[a][b] << endl;
		}
	}
	prepVec(vov);
}

//get the mins and max, reformat it into a single dimension vector
void ofApp::prepVec(vector<vector <int> > vov) {

	if (!searchChannel) {//channel A = unchecked
		vecMaxA = 0;
		vecMinA = 10000;
		vecItemsA = 0;
		spilledVecA.clear();

		for (int a = 0; a < vov.size(); a++) {
			for (int b = 0; b < vov[a].size(); b++) {
				if (vov[a][b] > vecMaxA) {
					vecMaxA = vov[a][b];
				}
				if (vov[a][b] < vecMinA) {
					vecMinA = vov[a][b];
				}
				vecItemsA++;
				spilledVecA.push_back(vov[a][b]);
			}
		}
		ofLogNotice("Vector Items A", ofToString(vecItemsA));
		ofLogNotice("Vector Max A", ofToString(vecMaxA));
		ofLogNotice("Vector Min A", ofToString(vecMinA));
	}
	else {//channel B = checked
		vecMaxB = 0;
		vecMinB = 10000;
		vecItemsB = 0;
		spilledVecB.clear();

		for (int a = 0; a < vov.size(); a++) {
			for (int b = 0; b < vov[a].size(); b++) {
				if (vov[a][b] > vecMaxB) {
					vecMaxB = vov[a][b];
				}
				if (vov[a][b] < vecMinB) {
					vecMinB = vov[a][b];
				}
				vecItemsB++;
				spilledVecB.push_back(vov[a][b]);
			}
		}
		ofLogNotice("Vector Items B", ofToString(vecItemsB));
		ofLogNotice("Vector Max B", ofToString(vecMaxB));
		ofLogNotice("Vector Min B", ofToString(vecMinB));
	}
	
	globalMax = MAX(vecMaxA, vecMaxB);
	globalMin = MIN(vecMinA, vecMinB);
}

//set the wave type: controlled  by keys "+" and "-"
float ofApp::waveSelector(float phase, int waveType) {
	switch (waveType) {
	case 1://sine
		return sin(phase*TWO_PI);
		break;
	case 2://square
		return sin(phase*TWO_PI)>0 ? 1 : -1;
		break;
	case 3://sawtooth
		return fmod(phase, TWO_PI);
		break;
	case 4://triangle
		return abs(sin(phase*TWO_PI));
		break;
	default:
		break;
	}
}

//print the wave type
void ofApp::stringWave() {
	switch (pickWave) {
	case 1:
		thisWave = "sine";
		break;
	case 2:
		thisWave = "square";
		break;
	case 3:
		thisWave = "sawtooth";
		break;
	case 4:
		thisWave = "triangle";
		break;
	}
}

//lerp the volume of each channel
void ofApp::changeSound(bool AB) {

	if (!AB) {
		destinationA = spilledVecA[lerpPlace];
		if (volScale) {
			//scale based on an all time max
			destinationA = (float)ofMap(destinationA, 0.0, 2500.0, 0.0, 1.0);
		}
		else {
			//scale in relation to search hits
			destinationA = (float)ofMap(destinationA, (float)globalMin, (float)globalMax, 0.0, 1.0);
		}

		//set lerp rate so each interval is constant
		lerpAmtA = (float)abs(prevDestA - destinationA)/ (float)lerpIncrement; //framerate is 60, this is 1 second intervals
		positionX = ofMap(lerpAmtA, 0.0, 1.0, 0.0, 330.0);//set position of the visuals
		volA = ofLerp(volA, (float)destinationA, lerpAmtA); //original lerpAmt was 0.02f
		volA = MAX(volA, 0.0);//keeps it above 0, for some reason it glitched out and went negative when the lerp amount was weird...

		//check if lerp is finished and increment it
		if (abs(destinationA - volA) < .01) {
			lerpPlace++;
			lerpStep = true;
			//ofLogNotice() << "new lerp " << lerpPlace <<"\n";
			positionZ = ofMap(lerpPlace, 0.0, (float)spilledVecA.size(), 0.0, 330.0);

			//loop it
			if (lerpPlace == spilledVecA.size()) {
				lerpPlace = 0;
			}
			prevDestA = destinationA;
		}
	}
	else {

		//must be before destinationB is changed
		if (lerpStep == true) {
			prevDestB = destinationB;
			lerpStep = false;
		}

		//ensures that it wont break if A is larger than B
		if (lerpPlace < spilledVecB.size()) {
			destinationB = spilledVecB[lerpPlace];
		}
		else {
			destinationB = 0.0;
		}

		if (volScale) {
			//scale based on an all time max
			destinationB = (float)ofMap(destinationB, 0.0, 2500.0, 0.0, 1.0);
		}
		else {
			//scale in relation to search hits
			destinationB = (float)ofMap(destinationB, (float)globalMin, (float)globalMax, 0.0, 1.0);
		}

		lerpAmtB = (float)abs(prevDestB - destinationB)/ (float)lerpIncrement; //framerate is 60, this is 1 second intervals
		positionXt = ofMap(lerpAmtB, 0.0, 1.0, 0.0, 530.0);
		volB = ofLerp(volB, (float)destinationB, lerpAmtB); //default was 0.02f
		volB = MAX(volB, 0.0);//for some reason it glitched out and went negative when the lerp amount was weird...
	}
}

//check if the channel needs to be muted
void ofApp::channelMute() {
	if (muteA) {
		muteAf = 0.0;
	}
	else {
		muteAf = 1.0;
	}

	if (muteB) {
		muteBf = 0.0;
	}
	else {
		muteBf = 1.0;
	}
}