#pragma once

// unique_ptr�g�p�ɂ�
#pragma warning(disable:4251)

#include <memory>
#include "AudioDefine.h"

namespace AudioLibrary {

	using namespace std;

	// �ǂݍ��񂾃T�E���h�̃f�[�^
	class AUDIOLIBRARY_API AudioData {
	public:
		~AudioData();

		// �R�s�[�͋֎~���邪�A���[�u�͋�����
		AudioData(const AudioData&) = delete;
		AudioData(AudioData&&) noexcept = default;
		AudioData& operator=(const AudioData&) = delete;
	};
}