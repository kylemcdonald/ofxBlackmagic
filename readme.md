# ofxBlackMagic is an addon for [openFrameworks](http://openframeworks.cc/)

This addon is derived from studying an older addon, [ofxBlackMagicGrabber](https://github.com/arturoc/ofxBlackmagicGrabber), and the [DeckLink SDK](http://www.blackmagicdesign.com/support/sdks) sample code.

The goal is to provide a simple and highly optimized interface with no extraneous methods that makes it clear what a Black Magic device can offer.

Some advantages of this addon:

* All DeckLink specific functionality is placed in `DeckLinkController.h`, which can be extended if you're interested in getting minimum latency by overloading the `VideoInputFrameArrived()` callback.
* Raw data is triple buffered to provide minimum delay to the DeckLink device and no tearing on the display side.
* YUV to RGB conversion is only done when the user requests to use or display RGB data.
* YUV to RGB conversion algorithm is optimized to increase memory locality.
* YUV to Grayscale conversion is provided for situations where color conversion is unnecessary.
* Raw YUV data is provided as a texture for decoding or processing with a shader.

## Supported System

This addon has been checked with OS X 10.8 on Mac Mini, openFrameworks 0.8.0 and a UltraStudio Mini Recorder with a single 1080p30 camera.

## Installation

First, install the [Black Magic software](http://www.blackmagicdesign.com/support). If you are using an UltraStudio Mini Recorder, you should download [Desktop Video 9.8 for Macintosh](http://www.blackmagicdesign.com/support/detail?sid=3958&pid=31781&leg=false&os=mac). After installation, go to System Preferences, click "Black Magic Design" and make sure "Use 1080p not 1080PsF" is checked (this option is only available when the capture card is plugged in). If you don't check this option, nothing will work.

Then go to the [support](http://www.blackmagicdesign.com/support/sdks) page and download the DeckLink SDK (currently at version 9.7.7). After unzipping the SDK open the app `Mac/Samples/bin/CapturePreview` and select the video format of your device and hit "Start". If you have the right mode selected you should see the video streaming.

One you see the demo app working, try building and running the `example/` that comes with `ofxBlackMagic`.