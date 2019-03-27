#pragma once

// unique_ptr�g�p�ɂ�
#pragma warning(disable:4251)

#include <memory>
#include <string>

#include "AudioDefine.h"
#include "AudioLoader.h"
#include "AudioPlayer.h"

namespace AudioLibrary {

	using namespace std;

	// �T�E���h�V�X�e�����Ǘ�����N���X
	class AUDIOLIBRARY_API Audio final sealed {

	public:

		static Audio& GetInstance();

		void Initialize();
		void Finalize();

		// �v���[���[���쐬����
		AudioPlayerHandler CreateAudioPlayer();

		// �����t�@�C����ǂݍ���
		shared_ptr<AudioData> LoadAudioData(const wstring& filePath);

	};
}