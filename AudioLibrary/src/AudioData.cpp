#include "stdafx.h"
#include "../include/AudioData.h"
#include <math.h>

namespace AudioLibrary {

	struct AudioData::Impl {
	public:
		const WAVEFORMATEX& _format;		// �g�`�̏��
		XAUDIO2_BUFFER _buffer;				// �\�[�X�{�C�X�̃o�b�t�@

		Impl(const WAVEFORMATEX& format, const XAUDIO2_BUFFER& buffer) :
			_buffer(buffer),
			_format(move(format)) { }
	};

	AudioData::AudioData(AudioData&&) noexcept = default;

	AudioData::AudioData(const WAVEFORMATEX& format, unique_ptr<uint8_t[]> wave, const XAUDIO2_BUFFER& buffer) :
		_impl(make_unique<Impl>(format, buffer)),
		_wave(move(wave)),
		_dataLength(buffer.AudioBytes),
		_sampleLength(buffer.AudioBytes / format.nBlockAlign) { }

	AudioData::~AudioData() {
		_wave.reset();
	}

	const unique_ptr<uint8_t[]>& AudioData::GetWaveFile() const {
		return _wave;
	}

	const uint8_t* AudioData::GetWaveData() const {
		return _impl->_buffer.pAudioData;
	}

	bool AudioData::ReadSamples(uint32_t start, uint32_t length, int** outSamples) const {
		if (length == 0) return false;
		if (start + length > _sampleLength) return false;

		auto byteCount = _impl->_format.wBitsPerSample / 8U;
		auto byteStart = start * byteCount;
		auto waveData = _impl->_buffer.pAudioData;

		for (size_t i = 0; i < length; i++) {

			int sample = 0;
			auto position = byteStart + i * byteCount;

			if (byteCount == 1) {
				// 1byte�̏ꍇ��0~255�͈̔͂�128�����S(�U���Ȃ�)
				sample = waveData[position] - 128;
			}
			else if (byteCount == 2) {
				// 2byte�̏ꍇ��-32768�`32767�͈̔͂�0�����S(�U���Ȃ�)

				// windows�ł̓��g���G���f�B�A���Ȃ̂ŁA�t����ǉ����Ă���
				short temp = waveData[position + 1];
				temp = temp << 8;
				temp |= waveData[position];

				// char�͈̔͂ɐ��^����
				//sample = (char)((float)temp / 256);
				sample = temp;
			}

			// �`��p�ɔ��]�H
			(*outSamples)[i] = sample * -1;
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