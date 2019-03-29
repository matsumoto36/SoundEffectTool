#include "stdafx.h"
#include "../include/AudioData.h"


namespace AudioLibrary {

	struct AudioData::Impl {
	public:
		const WAVEFORMATEX& _format;		// 波形の情報
		XAUDIO2_BUFFER _buffer;				// ソースボイスのバッファ

		Impl(const WAVEFORMATEX& format, const XAUDIO2_BUFFER& buffer) :
			_buffer(buffer),
			_format(move(format)) { }
	};

	AudioData::AudioData(AudioData&&) noexcept = default;

	AudioData::AudioData(const WAVEFORMATEX& format, unique_ptr<uint8_t[]> wave, const XAUDIO2_BUFFER& buffer) :
		_impl(make_unique<Impl>(format, buffer)),
		_wave(move(wave)) {}

	AudioData::~AudioData() {
		_wave.reset();
	}

	const WAVEFORMATEX& AudioData::GetFormat() const {
		return _impl->_format;
	}

	const XAUDIO2_BUFFER& AudioData::GetBuffer() const {
		return _impl->_buffer;
	}

	//const unique_ptr<uint8_t[]>& GetWave(int channel) const {

	//}
}