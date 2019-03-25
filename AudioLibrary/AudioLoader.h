#pragma once

#include <string>
#include <memory>

#include "AudioData.h"

namespace AudioLibrary {

	using namespace std;

	// �����t�@�C�������[�h����N���X
	class AUDIOLIBRARY_API AudioLoader final sealed {

		AudioLoader() {}
		~AudioLoader() {}

	public:
		
		// �����t�@�C����ǂݍ���
		static HRESULT LoadWaveFile(const unique_ptr<IXAudio2>& xAudio2, wstring& filePath, unique_ptr<AudioData>& retAudioData);

		// �t���p�X�ɕϊ�
		static bool ToFullPath(wstring& filepath, wstring& retFullPath);

	};

}