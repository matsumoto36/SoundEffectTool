#include "stdafx.h"
#include "../include/AudioData.h"
#include <math.h>

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
		_wave(move(wave)),
		_dataLength(buffer.AudioBytes),
		_sampleLength(buffer.AudioBytes / format.wBitsPerSample / 8) { }

	AudioData::~AudioData() {
		_wave.reset();
	}

	const unique_ptr<uint8_t[]>& AudioData::GetWaveFile() const {
		return _wave;
	}

	const uint8_t* AudioData::GetWaveData() const {
		return _impl->_buffer.pAudioData;
	}

	bool AudioData::ReadSamples(uint32_t start, uint32_t length, char** outSamples) const {
		if (length == 0) return false;
		if (start + length > _sampleLength) return false;

		auto byteCount = _impl->_format.wBitsPerSample / 8U;
		auto byteStart = start * byteCount;
		auto waveData = _impl->_buffer.pAudioData;

		for (size_t i = 0; i < length; i++) {

			char sample = 0;
			
			if (byteCount == 1) {
				// 1byteの場合は0~255の範囲で128が中心(振幅なし)
				sample = (char)waveData[byteStart];
			}
			else if (byteCount == 2) {
				// 2byteの場合は-32768～32767の範囲で0が中心(振幅なし)

				// windowsではリトルエンディアンなので、逆から追加していく
				uint16_t temp = waveData[byteStart + 1];
				temp = temp << 8;
				temp += waveData[byteStart];

				// charの範囲に成型する
				sample = (char)(temp / 256);
			}

			(*outSamples)[i] = sample;
		}

		return true;
	}

	const int AudioData::GetChannelCount() const {
		return _impl->_format.nChannels;
	}

	const WAVEFORMATEX& AudioData::GetFormat() const {
		return _impl->_format;
	}

	const XAUDIO2_BUFFER& AudioData::GetBuffer() const {
		return _impl->_buffer;
	}
}