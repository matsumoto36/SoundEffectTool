#pragma once

#include <memory>
#include <string>

#include "AudioDefine.h"
#include "AudioLoader.h"
#include "AudioPlayer.h"
#include "AudioResource.h"

namespace AudioLibrary {

	using namespace std;

	// �T�E���h�V�X�e�����Ǘ�����N���X
	class AUDIOLIBRARY_API Audio final sealed {

		static Audio& _instance;					// singleton
		static bool _isInitialized;					// ���������������Ă��邩

		IXAudio2* _xAudio2;							// XAudio2�̃V�X�e��
		IXAudio2MasteringVoice* _masteringVoice;	// �}�X�^�[�{�C�X

		Audio() {}
		~Audio() { Finalize(); }

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