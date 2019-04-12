#pragma once

// unique_ptr�g�p�ɂ�
#pragma warning(disable:4251)

#include <vector>
#include <memory>
#include "AudioDefine.h"

#include "../src/WAVFileReader.h"

using namespace std;

namespace AudioLibrary {

	// �ǂݍ��񂾃T�E���h�̃f�[�^
	class AUDIOLIBRARY_API AudioData {

		unique_ptr<uint8_t[]> _wave;	// �g�`�{��

		struct Impl;
		unique_ptr<Impl> _impl;

	public:
		AudioData(const WAVEFORMATEX& format, unique_ptr<uint8_t[]> wave, const XAUDIO2_BUFFER& buffer);
		~AudioData();

		// �R�s�[�͋֎~���邪�A���[�u�͋�����
		AudioData(const AudioData&) = delete;
		AudioData(AudioData&&) noexcept;
		AudioData& operator=(const AudioData&) = delete;

		// �w�肵�����̃f�[�^���擾
		const bool ReadData(size_t start, size_t count, const uint8_t* bytes) const;

		// �g�`��Ԃ�
		const unique_ptr<uint8_t[]>& GetWave() const;

		// �`�����l���̐���Ԃ�
		const int GetChannelCount() const;

		// �t�H�[�}�b�g��Ԃ�
		const WAVEFORMATEX& GetFormat() const;

		// �o�b�t�@��Ԃ�
		const XAUDIO2_BUFFER& GetBuffer() const;
	};
}