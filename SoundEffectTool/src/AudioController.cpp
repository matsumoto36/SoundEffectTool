#include "stdafx.h"
#include "../include/AudioController.h"

#include "AudioData.h"

namespace SoundEffectTool {

	struct AudioController::Impl {
		map<string, shared_ptr<AudioData>> _audioDataList;
	};

	AudioController::AudioController() :
		_impl(make_unique<Impl>()) {

		// �T�E���h�V�X�e���̏�����
		auto&& audio = Audio::GetInstance();
		audio.Initialize();
	}


	AudioController::~AudioController() {

		// �v���C���[�̔j��
		for (auto&& player : _audioPlayerList) {
			player.second.reset();
		}

		// �T�E���h�V�X�e���̔j��
		Audio::GetInstance().Finalize();
	
	}

	bool AudioController::CreateAudioPlayer(const string& key) {

		if (IsValidAudioPlayer(key)) {
			printf("already created player. key=%s", key.c_str());
			return false;
		}

		// �ǉ�
		_audioPlayerList.emplace(key, make_shared<AudioPlayer>());
		return true;
	}

	shared_ptr<AudioPlayer> AudioController::GetAudioPlayer(const string& key) {

		if (!IsValidAudioPlayer(key)) {
			printf("Player is not found. key=%s", key.c_str());
			return nullptr;
		}

		return _audioPlayerList[key];
	}

	shared_ptr<AudioData> AudioController::GetAudioData(const string& key) const {

		if (!IsValidAudioData(key)) {
			printf("Sound data is not found. key=%s", key.c_str());
			return nullptr;
		}

		return _impl->_audioDataList[key];
	}

	void AudioController::Update(float deltaTime) const {

		for (auto&& player : _audioPlayerList) {
			player.second->Update(deltaTime);
		}
	}

	bool AudioController::LoadSound(const wstring& filePath, const string& key) const {

		// �����L�[�œo�^�ł��Ȃ��悤�ɂ���
		if (IsValidAudioData(key)) {
			printf("already loaded sound data. key=%s", key.c_str());
			return false;
		};

		// �t�@�C���ǂݍ���
		auto audioData = move(Audio::GetInstance().LoadAudioData(filePath));

		if (!audioData) {
			wprintf(L"failed load sound data. path=%s", filePath.c_str());
			return false;
		}

		// �ǉ�
		_impl->_audioDataList.emplace(key, move(audioData));
		return true;
	}

	bool AudioController::UnLoadSound(const string& key) const {

		if (!IsValidAudioData(key)) {
			printf("Sound data is not found. key=%s", key.c_str());
			return false;
		}

		// �폜 (shared_ptr�Ȃ̂ŊJ�������\��)
		_impl->_audioDataList.erase(key);
		return true;
	}

	bool AudioController::IsValidAudioPlayer(const string& key) const {
		return _audioPlayerList.count(key) > 0;
	}

	bool AudioController::IsValidAudioData(const string& key) const {
		return _impl->_audioDataList.count(key) > 0;
	}


}