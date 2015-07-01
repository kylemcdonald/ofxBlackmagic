#pragma once

#include "ofMain.h"

#include "DeckLinkController.h"

class VideoFrame : public IDeckLinkVideoFrame, public ofBaseHasPixels {
private:
    ofPixels pixels;
    unsigned char* data;
    
    //override these methods for virtual
    virtual long GetWidth (void);
    virtual long GetHeight (void);
    virtual long GetRowBytes (void);
    virtual BMDPixelFormat GetPixelFormat (void);
    virtual BMDFrameFlags GetFlags (void);
    virtual HRESULT GetBytes (/* out */ void **buffer);
    
    //Dummy implementations of remaining virtual methods
    virtual HRESULT GetTimecode (/* in */ BMDTimecodeFormat format, /* out */ IDeckLinkTimecode **timecode) { return E_NOINTERFACE; };
    virtual HRESULT GetAncillaryData (/* out */ IDeckLinkVideoFrameAncillary **ancillary) { return E_NOINTERFACE; } ;
    
    // IUnknown interface (dummy implementation)
    virtual HRESULT QueryInterface (REFIID iid, LPVOID *ppv) {return E_NOINTERFACE;}
    virtual ULONG AddRef () {return 1;}
    virtual ULONG Release () {return 1;}

public:
    
    VideoFrame(long width, long height);
    virtual ~VideoFrame();
    
    void allocate(int width, int height);
    void deallocate();
    void copyFromFrame(IDeckLinkVideoFrame*);
    
    int getWidth();
    int getHeight();
    
    std::timed_mutex lock;
    
    unsigned char* getData();
    
    const ofPixels & getPixels() const;
    ofPixels & getPixels();
};
