
#include "DeckLinkController.h"

DeckLinkController::DeckLinkController()
: selectedDevice(NULL), deckLinkInput(NULL), supportFormatDetection(false), currentlyCapturing(false)  {}

DeckLinkController::~DeckLinkController()  {
	vector<IDeckLink*>::iterator it;
	
	// Release the IDeckLink list
	for(it = deviceList.begin(); it != deviceList.end(); it++) {
		(*it)->Release();
	}
}

bool DeckLinkController::init()  {
	IDeckLinkIterator* deckLinkIterator = NULL;
	IDeckLink* deckLink = NULL;
	bool result = false;
	
	// Create an iterator
	deckLinkIterator = CreateDeckLinkIteratorInstance();
	if (deckLinkIterator == NULL) {
		ofLogError("DeckLinkController") << "Please install the Blackmagic Desktop Video drivers to use the features of this application.";
		goto bail;
	}
	
	// List all DeckLink devices
	while (deckLinkIterator->Next(&deckLink) == S_OK) {
		// Add device to the device list
		deviceList.push_back(deckLink);
	}
	
	if (deviceList.size() == 0) {
		ofLogError("DeckLinkController") << "You will not be able to use the features of this application until a Blackmagic device is installed.";
		goto bail;
	}
	
	result = true;
	
bail:
	if (deckLinkIterator != NULL) {
		deckLinkIterator->Release();
		deckLinkIterator = NULL;
	}
	
	return result;
}


int DeckLinkController::getDeviceCount()  {
	return deviceList.size();
}

vector<string> DeckLinkController::getDeviceNameList()  {
	vector<string> nameList;
	int deviceIndex = 0;
	
	while (deviceIndex < deviceList.size()) {
		CFStringRef cfStrName;
		
		// Get the name of this device
		if (deviceList[deviceIndex]->GetDisplayName(&cfStrName) == S_OK) {
			nameList.push_back(string(CFStringGetCStringPtr(cfStrName, kCFStringEncodingMacRoman)));
			CFRelease(cfStrName);
		}
		else {
			nameList.push_back("DeckLink");
		}
		
		deviceIndex++;
	}
	
	return nameList;
}


bool DeckLinkController::selectDevice(int index)  {
	IDeckLinkAttributes* deckLinkAttributes = NULL;
	IDeckLinkDisplayModeIterator* displayModeIterator = NULL;
	IDeckLinkDisplayMode* displayMode = NULL;
	bool result = false;
	
	// Check index
	if (index >= deviceList.size()) {
		ofLogError("DeckLinkController") << "This application was unable to select the device.";
		goto bail;
	}
	
	// A new device has been selected.
	// Release the previous selected device and mode list
	if (deckLinkInput != NULL)
		deckLinkInput->Release();
	
	while(modeList.size() > 0) {
		modeList.back()->Release();
		modeList.pop_back();
	}
	
	
	// Get the IDeckLinkInput for the selected device
	if ((deviceList[index]->QueryInterface(IID_IDeckLinkInput, (void**)&deckLinkInput) != S_OK)) {
		ofLogError("DeckLinkController") << "This application was unable to obtain IDeckLinkInput for the selected device.";
		deckLinkInput = NULL;
		goto bail;
	}
	
	//
	// Retrieve and cache mode list
	if (deckLinkInput->GetDisplayModeIterator(&displayModeIterator) == S_OK) {
		while (displayModeIterator->Next(&displayMode) == S_OK)
			modeList.push_back(displayMode);
		
		displayModeIterator->Release();
	}
	
	//
	// Check if input mode detection format is supported.
	
	supportFormatDetection = false; // assume unsupported until told otherwise
	if (deviceList[index]->QueryInterface(IID_IDeckLinkAttributes, (void**) &deckLinkAttributes) == S_OK) {
		if (deckLinkAttributes->GetFlag(BMDDeckLinkSupportsInputFormatDetection, &supportFormatDetection) != S_OK)
			supportFormatDetection = false;
		
		deckLinkAttributes->Release();
	}
	
	result = true;
	
bail:
	return result;
}

vector<string> DeckLinkController::getDisplayModeNames()  {
	vector<string> modeNames;
	int modeIndex;
	CFStringRef modeName;
	
	for (modeIndex = 0; modeIndex < modeList.size(); modeIndex++) {
		if (modeList[modeIndex]->GetName(&modeName) == S_OK) {
			modeNames.push_back(string(CFStringGetCStringPtr(modeName, kCFStringEncodingMacRoman)));
			CFRelease(modeName);
		}
		else {
			modeNames.push_back("Unknown mode");
		}
	}
	
	return modeNames;
}

bool DeckLinkController::isFormatDetectionEnabled()  {
	return supportFormatDetection;
}

bool DeckLinkController::isCapturing()  {
	return currentlyCapturing;
}

unsigned long DeckLinkController::getDisplayModeBufferSize(BMDDisplayMode mode) {

	if(mode == bmdModeNTSC2398
			|| mode == bmdModeNTSC
			|| mode == bmdModeNTSCp) {
		return 720 * 486 * 2;
	} else if( mode == bmdModePAL
			|| mode == bmdModePALp) {
		return 720 * 576 * 2;
	} else if( mode == bmdModeHD720p50
			|| mode == bmdModeHD720p5994
			|| mode == bmdModeHD720p60) {
		return 1280 * 720 * 2;
	} else if( mode == bmdModeHD1080p2398
			|| mode == bmdModeHD1080p24
			|| mode == bmdModeHD1080p25
			|| mode == bmdModeHD1080p2997
			|| mode == bmdModeHD1080p30
			|| mode == bmdModeHD1080i50
			|| mode == bmdModeHD1080i5994
			|| mode == bmdModeHD1080i6000
			|| mode == bmdModeHD1080p50
			|| mode == bmdModeHD1080p5994
			|| mode == bmdModeHD1080p6000) {
		return 1920 * 1080 * 2;
	} else if( mode == bmdMode2k2398
			|| mode == bmdMode2k24
			|| mode == bmdMode2k25) {
		return 2048 * 1556 * 2;
	} else if( mode == bmdMode2kDCI2398
			|| mode == bmdMode2kDCI24
			|| mode == bmdMode2kDCI25) {
		return 2048 * 1080 * 2;
	} else if( mode == bmdMode4K2160p2398
			|| mode == bmdMode4K2160p24
			|| mode == bmdMode4K2160p25
			|| mode == bmdMode4K2160p2997
			|| mode == bmdMode4K2160p30) {
		return 3840 * 2160 * 2;
	} else if( mode == bmdMode4kDCI2398
			|| mode == bmdMode4kDCI24
			|| mode == bmdMode4kDCI25) {
		return 4096 * 2160 * 2;
	}

	return 0;
}

bool DeckLinkController::startCaptureWithIndex(int videoModeIndex)  {
	// Get the IDeckLinkDisplayMode from the given index
	if ((videoModeIndex < 0) || (videoModeIndex >= modeList.size())) {
		ofLogError("DeckLinkController") << "An invalid display mode was selected.";
		return false;
	}
	
	return startCaptureWithMode(modeList[videoModeIndex]->GetDisplayMode());
}

bool DeckLinkController::startCaptureWithMode(BMDDisplayMode videoMode) {
	unsigned long bufferSize = getDisplayModeBufferSize(videoMode);
	if (bufferSize == 0) {
		ofLogError("DeckLinkController") << "Invalid display mode";
	    return false;
	}
	vector<unsigned char> prototype(bufferSize);
	buffer.setup(prototype);

	BMDVideoInputFlags videoInputFlags;
	
	// Enable input video mode detection if the device supports it
	videoInputFlags = supportFormatDetection ? bmdVideoInputEnableFormatDetection : bmdVideoInputFlagDefault;
	
	// Set capture callback
	deckLinkInput->SetCallback(this);
	
	// Set the video input mode
	if (deckLinkInput->EnableVideoInput(videoMode, bmdFormat8BitYUV, videoInputFlags) != S_OK) {
		ofLogError("DeckLinkController") << "This application was unable to select the chosen video mode. Perhaps, the selected device is currently in-use.";
		return false;
	}
	
	// Start the capture
	if (deckLinkInput->StartStreams() != S_OK) {
		ofLogError("DeckLinkController") << "This application was unable to start the capture. Perhaps, the selected device is currently in-use.";
		return false;
	}
	
	currentlyCapturing = true;
	
	return true;
}

void DeckLinkController::stopCapture()  {
	// Stop the capture
	deckLinkInput->StopStreams();
	
	// Delete capture callback
	deckLinkInput->SetCallback(NULL);
	
	currentlyCapturing = false;
}


HRESULT DeckLinkController::VideoInputFormatChanged (/* in */ BMDVideoInputFormatChangedEvents notificationEvents, /* in */ IDeckLinkDisplayMode *newMode, /* in */ BMDDetectedVideoInputFormatFlags detectedSignalFlags)  {
	bool shouldRestartCaptureWithNewVideoMode = true;
	
	// Restart capture with the new video mode if told to
	if (shouldRestartCaptureWithNewVideoMode) {
		// Stop the capture
		deckLinkInput->StopStreams();
		
		// Set the video input mode
		if (deckLinkInput->EnableVideoInput(newMode->GetDisplayMode(), bmdFormat8BitYUV, bmdVideoInputEnableFormatDetection) != S_OK) {
			ofLogError("DeckLinkController") << "This application was unable to select the new video mode.";
			goto bail;
		}
		
		// Start the capture
		if (deckLinkInput->StartStreams() != S_OK) {
			ofLogError("DeckLinkController") << "This application was unable to start the capture on the selected device.";
			goto bail;
		}
	}
	
bail:
	return S_OK;
}

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

HRESULT DeckLinkController::VideoInputFrameArrived (/* in */ IDeckLinkVideoInputFrame* videoFrame, /* in */ IDeckLinkAudioInputPacket* audioPacket)  {
//	bool hasValidInputSource = (videoFrame->GetFlags() & bmdFrameHasNoInputSource) != 0;
	
//	AncillaryDataStruct ancillaryData;
	
	// Get the various timecodes and userbits for this frame
//	getAncillaryDataFromFrame(videoFrame, bmdTimecodeVITC, ancillaryData.vitcF1Timecode, ancillaryData.vitcF1UserBits);
//	getAncillaryDataFromFrame(videoFrame, bmdTimecodeVITCField2, ancillaryData.vitcF2Timecode, ancillaryData.vitcF2UserBits);
//	getAncillaryDataFromFrame(videoFrame, bmdTimecodeRP188VITC1, ancillaryData.rp188vitc1Timecode, ancillaryData.rp188vitc1UserBits);
//	getAncillaryDataFromFrame(videoFrame, bmdTimecodeRP188LTC, ancillaryData.rp188ltcTimecode, ancillaryData.rp188ltcUserBits);
//	getAncillaryDataFromFrame(videoFrame, bmdTimecodeRP188VITC2, ancillaryData.rp188vitc2Timecode, ancillaryData.rp188vitc2UserBits);

	void* bytes;
	videoFrame->GetBytes(&bytes);
	unsigned char* raw = (unsigned char*) bytes;
	vector<unsigned char>& back = buffer.getBack();
	back.assign(raw, raw + back.size());
	buffer.swapBack();
	
	return S_OK;
}

void DeckLinkController::getAncillaryDataFromFrame(IDeckLinkVideoInputFrame* videoFrame, BMDTimecodeFormat timecodeFormat, string& timecodeString, string& userBitsString)  {
	IDeckLinkTimecode* timecode = NULL;
	CFStringRef timecodeCFString;
	BMDTimecodeUserBits userBits = 0;
	
	if ((videoFrame != NULL)
		&& (videoFrame->GetTimecode(timecodeFormat, &timecode) == S_OK)) {
		if (timecode->GetString(&timecodeCFString) == S_OK) {
			timecodeString = string(CFStringGetCStringPtr(timecodeCFString, kCFStringEncodingMacRoman));
			CFRelease(timecodeCFString);
		}
		else {
			timecodeString = "";
		}
		
		timecode->GetTimecodeUserBits(&userBits);
		userBitsString = "0x" + ofToHex(userBits);
		
		timecode->Release();
	}
	else {
		timecodeString = "";
		userBitsString = "";
	}
	
	
}

// picks the mode with matching resolution, with highest available framerate
// and a preference for progressive over interlaced
BMDDisplayMode DeckLinkController::getDisplayMode(int w, int h) {

	if (w == 720 && h == 486) {				// NTSC
		return bmdModeNTSCp;
	} else if (w == 720 && h == 576) {		// PAL
		return bmdModePALp;
	} else if (w == 1280 && h == 720) {		// HD 720
		return bmdModeHD720p60;
	} else if (w == 1920 && h == 1080) {	// HD 1080
		return bmdModeHD1080p6000;
	} else if (w == 2048 && h == 1556) {	// 2k
		return bmdMode2k25;
	} else if (w == 2048 && h == 1080) {	// 2k DCI
		return bmdMode2kDCI25;
	} else if (w == 3840 && h == 2160) {	// 4K
		return bmdMode4K2160p30;
	} else if (w == 4096 && h == 2160) {	// 4k DCI
		return bmdMode4kDCI25;
	}

	return bmdModeUnknown;
}

// framerate should be in the form of thousands for decimal values
// e.g. 2997 for 29.97fps
// for whole numbers it can be either
// e.g. 30 or 3000 for 30fps
// that said, with int trucation we can make some assumptions
BMDDisplayMode DeckLinkController::getDisplayMode(int w, int h, int framerate) {
	string err = "invalid framerate, for this resolution you can use:";

	if (w == 720 && h == 486) {									// NTSC
		if (framerate == 2997 || framerate == 29) {
		    return bmdModeNTSC;
		} else if (framerate == 2398 || framerate == 23) {
            return bmdModeNTSC2398;
		} else if (framerate == 5994 || framerate == 59) {
		    return bmdModeNTSCp;
		} else {
			ofLogError("DeckLinkController") << err << endl
				<< "29, 23, 59, 2997, 2398, 5994";
			return bmdModeUnknown;
		}
	} else if (w == 720 && h == 576) {							// PAL
		if (framerate == 2500 || framerate == 25) {
		    return bmdModePAL;
		} else if (framerate == 5000 || framerate == 50) {
		    return bmdModePALp;
		} else {
			ofLogError("DeckLinkController") << err << endl
				<< "25, 50, 2500, 5000";
			return bmdModeUnknown;
		}
	} else if (w == 1280 && h == 720) {							// HD 720
		if (framerate == 5000 || framerate == 50) {
            return bmdModeHD720p50;
		} else if (framerate == 5994 || framerate == 59) {
		    return bmdModeHD720p5994;
		} else if (framerate == 6000 || framerate == 60) {
			return bmdModeHD720p60;
		} else {
			ofLogError("DeckLinkController") << err << endl
				<< "59, 60, 5994, 6000";
			return bmdModeUnknown;
		}
	} else if (w == 1920 && h == 1080) {						// HD 1080
		if (framerate == 2398 || framerate == 23) {
			return bmdModeHD1080p2398;
		} else if (framerate == 2400 || framerate == 24) {
			return bmdModeHD1080p24;
		} else if (framerate == 2500 || framerate == 25) {
			return bmdModeHD1080p25;
		} else if (framerate == 2997 || framerate == 29) {
			return bmdModeHD1080p2997;
		} else if (framerate == 3000 || framerate == 30) {
			return bmdModeHD1080p30;
		} else if (framerate == 5000 || framerate == 50) {
			return bmdModeHD1080i50;
		} else if (framerate == 5994 || framerate == 59) {
			return bmdModeHD1080i5994;
		} else if (framerate == 6000 || framerate == 60) {
			return bmdModeHD1080i6000;
		} else if (framerate == 5000 || framerate == 50) {
			return bmdModeHD1080p50;
		} else if (framerate == 5994 || framerate == 59) {
			return bmdModeHD1080p5994;
		} else if (framerate == 6000 || framerate == 60) {
			return bmdModeHD1080p6000;
		} else {
			ofLogError("DeckLinkController") << err << endl
				<< "24, 25, 29, 30, 50, 59, 60, 50, 59, 60" << endl
				<< "2400, 2500, 2997, 3000, 5000, 5994, 6000, 5000, 5994, 6000";
			return bmdModeUnknown;
		}
	} else if (w == 2048 && h == 1556) {						// 2k
		if (framerate == 2398 || framerate == 23) {
			return bmdMode2k2398;
		} else if (framerate == 2400 || framerate == 24) {
			return bmdMode2k24;
		} else if (framerate == 2500 || framerate == 25) {
			return bmdMode2k25;
		} else {
			ofLogError("DeckLinkController") << err << endl
				<< "23, 24, 25, 2398, 2400, 2500";
			return bmdModeUnknown;
		}
	} else if (w == 2048 && h == 1080) {						// 2k DCI
		if (framerate == 2398 || framerate == 23) {
			return bmdMode2kDCI2398;
		} else if (framerate == 2400 || framerate == 24) {
			return bmdMode2kDCI24;
		} else if (framerate == 2500 || framerate == 25) {
			return bmdMode2kDCI25;
		} else {
			ofLogError("DeckLinkController") << err << endl
				<< "23, 24, 25, 2398, 2400, 2500";
			return bmdModeUnknown;
		}
	} else if (w == 3840 && h == 2160) {						// 4K
		if (framerate == 2398 || framerate ==  23) {
			return bmdMode4K2160p2398;
		} else if (framerate == 2400 || framerate == 24) {
			return bmdMode4K2160p24;
		} else if (framerate == 2500 || framerate == 25) {
			return bmdMode4K2160p25;
		} else if (framerate == 2997 || framerate == 29) {
			return bmdMode4K2160p2997;
		} else if (framerate == 3000 || framerate == 30) {
			return bmdMode4K2160p30;
		} else {
			ofLogError("DeckLinkController") << err << endl
				<< "23, 24, 25, 29, 30" << endl
				<< "2398, 2400, 2500, 2997, 3000";
			return bmdModeUnknown;
		}
	} else if (w == 4096 && h == 2160) {						// 4k DCI
		if (framerate == 2398 || framerate == 23) {
		    return bmdMode4kDCI2398;
		} else if (framerate == 2400 || framerate == 24) {
		    return bmdMode4kDCI24;
		} else if (framerate == 2500 || framerate == 25) {
		    return bmdMode4kDCI25;
		} else {
			ofLogError("DeckLinkController") << err << endl
				<< "23, 24, 25, 2398, 2400, 2500";
			return bmdModeUnknown;
		}
	}

	return bmdModeUnknown;
}
