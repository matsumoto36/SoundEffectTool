#pragma once

// unique_ptr�g�p�ɂ�
#pragma warning(disable:4251)

#include <memory>
#include "AudioDefine.h"

namespace AudioLibrary {

	using namespace std;

	// �ǂݍ��񂾃T�E���h�̃f�[�^
	class AUDIOLIBRARY_API AudioData {

		unique_ptr<WAVEFORMATEX> _format;				// �g�`�̏��
		unique_ptr<uint8_t[]> _wave;					// �g�`�{��
		XAUDIO2_BUFFER _buffer;						// �\�[�X�{�C�X�̃o�b�t�@

	public:
		AudioData(unique_ptr<WAVEFORMATEX> format, unique_ptr<uint8_t[]> wave, XAUDIO2_BUFFER& buffer);
		~AudioData();

		// �R�s�[�͋֎~���邪�A���[�u�͋�����
		AudioData(const AudioData&) = delete;
		AudioData(AudioData&&) noexcept = default;
		AudioData& operator=(const AudioData&) = delete;

		// �t�H�[�}�b�g��Ԃ�
		const WAVEFORMATEX& GetFormat() const {
			return *_format;
		}

		// �o�b�t�@��Ԃ�
		const XAUDIO2_BUFFER& GetBuffer() const {
			return _buffer;
		}
	};
}