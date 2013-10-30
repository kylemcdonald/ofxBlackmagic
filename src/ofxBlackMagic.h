/*
 Everything in ofxBlackMagic is lazy, and only gets allocated or converted when
 you ask for it. "yuv" really means "cb y0 cr y1", i.e., two pixels stored in
 four bytes.
 */

#pragma once

#include "ofMain.h"

#include "DeckLinkController.h"

class ofxBlackMagic {
private:
	DeckLinkController controller;
	
	bool grayPixOld, colorPixOld;
	ofPixels yuvPix, grayPix, colorPix;
	bool yuvTexOld, grayTexOld, colorTexOld;
	ofTexture yuvTex, grayTex, colorTex;
	
	int width, height;
	
public:
	ofxBlackMagic();
	bool setup(int width, int height, float framerate);
	void close(); // should call this in ofApp::exit()
	bool update(); // returns true if there is a new frame
	
	vector<unsigned char>& getYuvRaw(); // fastest
	ofPixels& getGrayPixels(); // fast
	ofPixels& getColorPixels(); // slow
	
	ofTexture& getYuvTexture(); // fastest
	ofTexture& getGrayTexture(); // fast
	ofTexture& getColorTexture(); // slower
	
	void drawYuv(); // fastest
	void drawGray(); // fast
	void drawColor(); // slower
};