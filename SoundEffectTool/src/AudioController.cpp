#include "stdafx.h"
#include "../include/AudioController.h"

#include "AudioData.h"

namespace SoundEffectTool {

	struct AudioController::Impl {
		map<string, shared_ptr<AudioData>> _audioDataList;
	};

	AudioController::AudioController() :
		_impl(make_unique<Impl>()) {

		// サウンドシステムの初期化
		auto&& audio = Audio::GetInstance();
		audio.Initialize();
	}


	AudioController::~AudioController() {

		// プレイヤーの破棄
		for (auto&& player : _audioPlayerList) {
			player.second.reset();
		}

		// サウンドシステムの破棄
		Audio::GetInstance().Finalize();
	
	}

	bool AudioController::CreateAudioPlayer(const string& key) {

		if (IsValidAudioPlayer(key)) {
			printf("already created player. key=%s", key.c_str());
			return false;
		}

		// 追加
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

		// 同じキーで登録できないようにする
		if (IsValidAudioData(key)) {
			printf("already loaded sound data. key=%s", key.c_str());
			return false;
		};

		// ファイル読み込み
		auto audioData = move(Audio::GetInstance().LoadAudioData(filePath));

		if (!audioData) {
			wprintf(L"failed load sound data. path=%s", filePath.c_str());
			return false;
		}

		// 追加
		_impl->_audioDataList.emplace(key, move(audioData));
		return true;
	}

	bool AudioController::UnLoadSound(const string& key) const {

		if (!IsValidAudioData(key)) {
			printf("Sound data is not found. key=%s", key.c_str());
			return false;
		}

		// 削除 (shared_ptrなので開放される予定)
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