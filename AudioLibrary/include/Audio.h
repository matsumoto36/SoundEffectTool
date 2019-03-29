#pragma once

#include <memory>
#include <string>

#include "AudioDefine.h"
#include "AudioLoader.h"
#include "AudioPlayer.h"

using namespace std;

namespace AudioLibrary {

	// �f���[�^�[
	struct XAudio2Deleter {
		void operator()(IXAudio2* xaudio2) const;
	};

	struct MasteringVoiceDeleter {
		void operator()(IXAudio2MasteringVoice* masteringVoice) const;
	};

	// �T�E���h�V�X�e�����Ǘ�����N���X
	class AUDIOLIBRARY_API Audio final sealed {

		static Audio& _instance;					// singleton
		static bool _isInitialized;					// ���������������Ă��邩

		unique_ptr<IXAudio2, XAudio2Deleter> _xAudio2;								// XAudio2�̃V�X�e��
		unique_ptr<IXAudio2MasteringVoice, MasteringVoiceDeleter> _masteringVoice;	// �}�X�^�[�{�C�X

		Audio();
		~Audio();

	public:

		static Audio& GetInstance();

		void Initialize();
		void Finalize();

		// �v���[���[���쐬����
		unique_ptr<AudioPlayer, AudioPlayerDeleter> CreateAudioPlayer();

		// �����t�@�C����ǂݍ���
		shared_ptr<AudioData> LoadAudioData(const wstring& filePath);

	};
}