#include "VideoFrame.h"

VideoFrame::VideoFrame(long width, long height)
{
    this->data = 0;
    allocate(width, height);
}

VideoFrame::~VideoFrame()
{
    this->deallocate();
}

void VideoFrame::allocate(int width, int height) {
    this->deallocate();
    //we offset to store 1 byte either side of RGB so we can do ARGB and RGBA addressing
    this->data = new unsigned char[width * height * 4 + 1];
    this->pixels.setFromExternalPixels(this->data + 1, width, height, 4);
    *(data + this->pixels.size()) = 255; // set alpha channel of last byte
}

void VideoFrame::deallocate() {
    if (this->data != 0) {
        delete[] this->data;
        this->data = 0;
    }
}

int VideoFrame::getWidth() {
    return this->pixels.getWidth();
}

int VideoFrame::getHeight() {
    return this->pixels.getHeight();
}

unsigned char* VideoFrame::getData() {
    return this->pixels.getData();
}

ofPixels& VideoFrame::getPixels() {
    return this->pixels;
}

const ofPixels& VideoFrame::getPixels() const {
    return this->pixels;
}

long VideoFrame::GetWidth() {
    return this->pixels.getWidth();
}

long VideoFrame::GetHeight() {
    return this->pixels.getHeight();
}

long VideoFrame::GetRowBytes() {
    return this->GetWidth() * 4;
}

BMDPixelFormat VideoFrame::GetPixelFormat() {
    return bmdFormat8BitARGB;
}

BMDFrameFlags VideoFrame::GetFlags() {
    return 0;
}

HRESULT VideoFrame::GetBytes(void **buffer) {
    *buffer = this->data;
    return S_OK;
}