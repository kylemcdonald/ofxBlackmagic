# ofxBlackMagic is an addon for [openFrameworks](http://openframeworks.cc/)

updated to support 3840x2160 @ 29.97 input from the ULTRASTUDIO 4K


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

This addon has been checked with OS X 10.8 on Mac Mini, openFrameworks 0.8.0 and an UltraStudio Mini Recorder with a single 1080p30 camera.

This addon has been checked with a OS X 10.9 on a MacPro trashcan with openFrameworks 0.8.3 and an UltraStudio 4k Thunderbolt 2 device. 

## Installation

First, install the [Black Magic software](http://www.blackmagicdesign.com/support). If you are using an UltraStudio Mini Recorder, you should download "Desktop Video 10.8.1 for Macintosh". After installation, go to System Preferences, click "Black Magic Desktop Video", click on your device, and make sure "1080PsF" is un-checked (this option is only available when the capture card is plugged in). If you don't un-check this option, nothing will work.

Then go to the [support](http://www.blackmagicdesign.com/support/sdks) page and download the DeckLink SDK (currently at version 10.8). After unzipping the SDK open the app `Mac/Samples/bin/CapturePreview` and hit "Start". The app should automatically detect the mode of your camera.

One you see the demo app working, try building and running the `example/` that comes with `ofxBlackMagic`.
