#include "stdafx.h"
#include "AudioData.h"

namespace AudioLibrary {

	AudioData::AudioData(unique_ptr<WAVEFORMATEX> format, unique_ptr<uint8_t[]> wave, XAUDIO2_BUFFER& buffer) :
		_format(move(format)),
		_wave(move(wave)),
		_buffer(buffer) {}

	AudioData::~AudioData() {
		//if (_buffer) delete _buffer;
	}
}