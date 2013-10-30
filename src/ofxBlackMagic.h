#include "ofMain.h"

#include "DeckLinkController.h"

inline void cby0cry1_y(unsigned char* cby0cry1, unsigned char* y, unsigned int n) {
	for(unsigned int i = 0, j = 1; i < n; i++, j += 2) {
		y[i] = cby0cry1[j];
	}
}

class ofxBlackMagic {
private:
	bool newFrame;
	ofImage img;
	DeckLinkController controller;
	
public:
	ofxBlackMagic()
	:newFrame(false)
	{}
	bool setup(int width, int height, float framerate) {
		if(!controller.init()) {
			return false;
		}
		controller.selectDevice(0);
		vector<string> displayModes = controller.getDisplayModeNames();
		ofLogVerbose("ofxBlackMagic") << "Available display modes: " << ofToString(displayModes);
		BMDDisplayMode displayMode = bmdModeUnknown;
		if(width == 1920 && height == 1080) {
			if(framerate == 30) {
				displayMode = bmdModeHD1080p30;
			}
		}
		img.allocate(1920, 1080, OF_IMAGE_GRAYSCALE);
		controller.startCaptureWithMode(displayMode);
		return true;
	}
	void close() {
		if(controller.isCapturing()) {
			controller.stopCapture();
		}
	}
	bool update() {
		if(controller.buffer.swapFront()) {
			int n = img.getWidth() * img.getHeight();
			cby0cry1_y(&(controller.buffer.getFront()[0]), img.getPixels(), n);
			img.update();
			return true;
		} else {
			return false;
		}
	}
	void draw() {
		if(img.isAllocated()) {
			img.draw(0, 0);
		}
	}
	bool isFrameNew() {
		bool curNewFrame = newFrame;
		newFrame = false;
		return curNewFrame;
	}
};