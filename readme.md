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