#include "ofMain.h"

template <class T>
class TripleBuffer : public ofMutex {
private:
	T back, middle, front;
	bool newData;
public:
	TripleBuffer()
	:newData(false) {
	}
	void setup(const T& prototype) {
		back = prototype;
		middle = prototype;
		front = prototype;
	}
	T& getBack() {
		return back;
	}
	T& getFront() {
		return front;
	}
	void swapBack() {
		lock();
		swap(back, middle);
		newData = true;
		unlock();
	}
	bool swapFront() {
		lock();
		bool curNewData = newData;
		if(newData) {
			swap(front, middle);
			newData = false;
		}
		unlock();
		return curNewData;
	}
};

template <class T>
class FixedMemoryCircularBuffer {
private:
	vector<T> buffer;
	unsigned int readPosition, writePosition, size;
public:
	FixedMemoryCircularBuffer()
	:size(0)
	,readPosition(0)
	,writePosition(0) {
	}
	void setup(int size, const T& prototype = T()) {
		buffer.resize(size, prototype);
	}
	bool canWrite() const {
		return size < buffer.size();
	}
	T& beginWrite() {
		return buffer[writePosition];
	}
	void endWrite() {
		writePosition = (writePosition + 1) % buffer.size();
		size++;
	}
	bool canRead() const {
		return size > 0;
	}
	T& beginRead() {
		return buffer[readPosition];
	}
	void endRead() {
		readPosition = (readPosition + 1) % buffer.size();
		size--;
	}
};

template <class T>
class SynchronizedQueue : public ofMutex {
private:
	queue<T> buffer;
	unsigned int maximumSize;
public:
	SynchronizedQueue():
	maximumSize(0) {
	}
	void push(T& element) {
		lock();
		buffer.push_back(element);
		if(maximumSize && buffer.size() > maximumSize) {
			buffer.pop_front();
		}
		unlock();
	}
	T pop() {
		lock();
		T element = buffer.pop_front();
		unlock();
		return element;
	}
};