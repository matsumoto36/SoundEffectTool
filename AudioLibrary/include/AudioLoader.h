#pragma once

#include <string>
#include <memory>

#include "AudioDefine.h"
#include "AudioData.h"
#include "../src/WAVFileReader.h"

namespace AudioLibrary {

	using namespace std;

	// �����t�@�C�������[�h����N���X
	class AUDIOLIBRARY_API AudioLoader final sealed {

		AudioLoader() {}
		~AudioLoader() {}

	public:
		
		// �����t�@�C����ǂݍ���
		static HRESULT LoadWaveFile(const IXAudio2& xAudio2, const wstring& filePath, shared_ptr<AudioData>& retAudioData);

	private:

		// �t���p�X�ɕϊ�
		static bool ToFullPath(const wstring& filepath, wstring& retFullPath);

	};

}