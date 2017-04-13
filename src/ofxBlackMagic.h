/*
 Everything in ofxBlackMagic is lazy, and only gets allocated or converted when
 you ask for it. "yuv" really means "cb y0 cr y1", i.e., two pixels stored in
 four bytes.
 */

#pragma once

#include "ofMain.h"

#include "DeckLinkController.h"

class ofxBlackMagic {
public:
    // Color Frame Capture Mode
    // A lock is necessary to prevent reading from the VideoFrame whilst writing to it.
    // A value 500 milliseconds would be for a 'frame critical' application, as a value of 75 milliseconds would be for a 'latency critical' application.
    // See PR #8 - https://github.com/kylemcdonald/ofxBlackmagic/pull/8
    enum ColorFrameCaptureMode {
        LOW_LATENCY = 75,
        NO_FRAME_DROPS = 500
    };
    
    ofxBlackMagic();
    bool setup(int width, int height, float framerate, int deviceId = 0, ColorFrameCaptureMode colorFrameCaptureMode = LOW_LATENCY);
    bool setup(BMDDisplayMode displayMode, int deviceId = 0, ColorFrameCaptureMode colorFrameCaptureMode = LOW_LATENCY);
    void setColorFrameCaptureMode(ColorFrameCaptureMode colorFrameCaptureMode); // If you want to set a custom value, you just need to cast it as ofxBlackMagic::ColorFrameCaptureMode
    ColorFrameCaptureMode getColorFrameCaptureMode();
    
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
    
private:
	DeckLinkController controller;
	
	bool grayPixOld, colorPixOld;
	ofPixels yuvPix, grayPix, colorPix;
	bool yuvTexOld, grayTexOld, colorTexOld;
	ofTexture yuvTex, grayTex, colorTex;
	
	int width, height;
    ColorFrameCaptureMode colorFrameCaptureMode;
};
