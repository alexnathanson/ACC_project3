#pragma once
#include "ofxJSON.h"
#include "ofThread.h"

class jsonThread : public ofThread {

	public:
		jsonThread();
		~jsonThread();

		string urlT;

		bool parsingT = false;

		ofxJSONElement jsonT;

		//Json::Value hits;
		int hits;

		string input;
		string begin;
		string end;
		string searchKey;

		bool done;

		void setup(string inputS, string beginS, string endS, string searchKeyS) {
			input = inputS;
			begin = beginS;
			end = endS;
			searchKey = searchKeyS;

			done = false;
		}

		void threadedFunction() {
			
			//ofLogNotice("Thread", "Threading!");

			if (!done) {
				urlT = "http://api.nytimes.com/svc/search/v2/articlesearch.json?q=" + input + "&facet_field=source&begin_date=" + begin + "&end_date=" + end + "&fl=headline&api-key=" + searchKey;
				parsingT = jsonT.open(urlT);
				sleep(2000);
			}
			
			
			if (parsingT)
			{
				hits = jsonT["response"]["meta"]["hits"].asInt();
				done = true;
				//ofLogNotice("Hits", std::toString(hits));
				//parsingT == false;
			}
			else {
				ofLogNotice("ofApp::setup") << "Failed to parse JSON.";
			}
		}
};