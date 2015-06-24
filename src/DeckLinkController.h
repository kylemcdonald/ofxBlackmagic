/*
 DeckLinkController is a C++ only port of sample code from the DeckLink SDK
 that demonstrates how to get device info, start and stop a capture stream, and
 get video frame data from an input device. The only addition is a triple
 buffered video data member.
 */

#pragma once

#include "ofMain.h"

#include "DeckLinkAPI.h"
#include "TripleBuffer.h"
#include "VideoFrame.h"

// Lock timeout duration.
// The lock is necessary to prevent reading from the VideoFrame whilst writing to it.
// A value 500 milliseconds would be for a 'frame critical' application, as a value of 66 milliseconds would be for a 'latency critical' application.
// Run tests to determine what timeout value you need.
// See PR #8 - https://github.com/kylemcdonald/ofxBlackmagic/pull/8
#define VIDEO_CONVERSION_TRYLOCK_TIMEOUT 500

class VideoFrame;

class DeckLinkController : public IDeckLinkInputCallback {
private:
	vector<IDeckLink*> deviceList;
	IDeckLink* selectedDevice;
	IDeckLinkInput* deckLinkInput;
	vector<IDeckLinkDisplayMode*> modeList;
	
	bool supportFormatDetection;
	bool currentlyCapturing;
    
    IDeckLinkVideoConversion *videoConverter;
	
	void getAncillaryDataFromFrame(IDeckLinkVideoInputFrame* frame, BMDTimecodeFormat format, string& timecodeString, string& userBitsString);
	
public:
	TripleBuffer< vector<unsigned char> > buffer;
	
	DeckLinkController();
	virtual ~DeckLinkController();
	
	bool init();
	
	int getDeviceCount();
	vector<string> getDeviceNameList();
	
	bool selectDevice(int index);
	
	vector<string> getDisplayModeNames();
	bool isFormatDetectionEnabled();
	bool isCapturing();

	unsigned long getDisplayModeBufferSize(BMDDisplayMode mode);

	bool startCaptureWithMode(BMDDisplayMode videoMode);
	bool startCaptureWithIndex(int videoModeIndex);
	void stopCapture();
    
	virtual HRESULT QueryInterface (REFIID iid, LPVOID *ppv) {return E_NOINTERFACE;}
	virtual ULONG AddRef () {return 1;}
	virtual ULONG Release () {return 1;}
    
	virtual HRESULT VideoInputFormatChanged (/* in */ BMDVideoInputFormatChangedEvents notificationEvents, /* in */ IDeckLinkDisplayMode *newDisplayMode, /* in */ BMDDetectedVideoInputFormatFlags detectedSignalFlags);
	virtual HRESULT VideoInputFrameArrived (/* in */ IDeckLinkVideoInputFrame* videoFrame, /* in */ IDeckLinkAudioInputPacket* audioPacket);
    
	BMDDisplayMode getDisplayMode(int w, int h);
	BMDDisplayMode getDisplayMode(int w, int h, float framerate);
    
    VideoFrame *rgbaFrame;
};