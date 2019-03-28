#pragma once

#include <memory>
#include <string>

#include "AudioLoader.h"
#include "AudioPlayer.h"

namespace AudioLibrary {

	using namespace std;

	// �T�E���h�V�X�e�����Ǘ�����N���X
	class AUDIOLIBRARY_API Audio final sealed {

		static Audio& _instance;					// singleton
		static bool _isInitialized;					// ���������������Ă��邩

		class Impl;
		unique_ptr<Impl> _impl;

		Audio();
		~Audio() { Finalize(); }

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