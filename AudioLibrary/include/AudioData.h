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

		unique_ptr<uint8_t[]> _wave;	// �t�@�C���f�[�^�{��
		uint32_t _dataLength;			// �g�`�̒���(�o�C�g��)
		uint32_t _sampleLength;			// �g�`�̒���(�T���v����)

		struct Impl;
		unique_ptr<Impl> _impl;

	public:
		AudioData(const WAVEFORMATEX& format, unique_ptr<uint8_t[]> wave, const XAUDIO2_BUFFER& buffer);
		~AudioData();

		// �R�s�[�͋֎~���邪�A���[�u�͋�����
		AudioData(const AudioData&) = delete;
		AudioData(AudioData&&) noexcept;
		AudioData& operator=(const AudioData&) = delete;

		// �g�`�t�@�C���f�[�^��Ԃ�
		const unique_ptr<uint8_t[]>& GetWaveFile() const;

		// �g�`�f�[�^��Ԃ�
		const uint8_t* GetWaveData() const;

		// �g�`�̃f�[�^�̒�����Ԃ�
		const uint32_t GetDataLength() const {
			return _dataLength;
		}

		// �g�`�̃T���v������Ԃ�
		const uint32_t GetSampleLength() const {
			return _sampleLength;
		}

		// �g�`���̃T���v����ǂݎ��
		bool ReadSamples(uint32_t start, uint32_t length, char** outSamples) const;

		// �`�����l���̐���Ԃ�
		const int GetChannelCount() const;

		// �t�H�[�}�b�g��Ԃ�
		const WAVEFORMATEX& GetFormat() const;

		// �o�b�t�@��Ԃ�
		const XAUDIO2_BUFFER& GetBuffer() const;
	};
}