#include "ofMain.h"

#include "DeckLinkAPI.h"
#include "Synchronized.h"

typedef struct {
	// VITC timecodes and user bits for field 1 & 2
	string vitcF1Timecode;
	string vitcF1UserBits;
	string vitcF2Timecode;
	string vitcF2UserBits;
	
	// RP188 timecodes and user bits (VITC1, VITC2 and LTC)
	string rp188vitc1Timecode;
	string rp188vitc1UserBits;
	string rp188vitc2Timecode;
	string rp188vitc2UserBits;
	string rp188ltcTimecode;
	string rp188ltcUserBits;
} AncillaryDataStruct;

class DeckLinkController : public IDeckLinkInputCallback {
private:
	vector<IDeckLink*> deviceList;
	IDeckLink* selectedDevice;
	IDeckLinkInput* deckLinkInput;
	vector<IDeckLinkDisplayMode*> modeList;
	
	bool supportFormatDetection;
	bool currentlyCapturing;
	
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
	
	bool startCaptureWithMode(BMDDisplayMode videoMode);
	bool startCaptureWithIndex(int videoModeIndex);
	void stopCapture();
	
	virtual HRESULT QueryInterface (REFIID iid, LPVOID *ppv) {return E_NOINTERFACE;}
	virtual ULONG AddRef () {return 1;}
	virtual ULONG Release () {return 1;}
	
	virtual HRESULT VideoInputFormatChanged (/* in */ BMDVideoInputFormatChangedEvents notificationEvents, /* in */ IDeckLinkDisplayMode *newDisplayMode, /* in */ BMDDetectedVideoInputFormatFlags detectedSignalFlags);
	virtual HRESULT VideoInputFrameArrived (/* in */ IDeckLinkVideoInputFrame* videoFrame, /* in */ IDeckLinkAudioInputPacket* audioPacket);
};