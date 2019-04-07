#include "stdafx.h"
#include "../include/AudioController.h"

#include <map>
#include "AudioPlayer.h"
#include "AudioData.h"

namespace SoundEffectTool {

	struct AudioController::Impl {
		map<string, unique_ptr<AudioPlayer, AudioPlayerDeleter>> _audioPlayerList;
		map<string, shared_ptr<AudioData>> _audioDataList;
	};

	AudioController::AudioController() :
		_impl(make_unique<Impl>()) {

		// サウンドシステムの初期化
		auto&& audio = Audio::GetInstance();
		audio.Initialize();

		// プレーヤーを作る
		auto player = move(audio.CreateAudioPlayer());
		
		// 通知をリンク
		player->OnIsPlayChanged = [&](bool isPlay) { OnIsPlayChanged(isPlay); };
		player->OnVolumeChanged = [&](float volume) { OnVolumeChanged(volume); };

		// メインとして追加
		_impl->_audioPlayerList.emplace("Main", move(player));
	}


	AudioController::~AudioController() {

		// プレイヤーの破棄
		for (auto&& player : _impl->_audioPlayerList) {
			player.second.reset();
		}

		// サウンドシステムの破棄
		Audio::GetInstance().Finalize();
	
	}

	bool AudioController::IsPlay() const {
		return _impl->_audioPlayerList["Main"]->IsPlay();
	}

	void AudioController::Update() const {
		_impl->_audioPlayerList["Main"]->Update();
	}

	bool AudioController::LoadSound(const wstring& filePath, const string& name) const {

		// 同じキーで登録できないようにする
		if (_impl->_audioDataList.count(name) > 0) {
			printf("already loaded sound data. key=%s", name.c_str());
		};

		// ファイル読み込み
		auto audioData = move(Audio::GetInstance().LoadAudioData(filePath));

		if (!audioData) {
			wprintf(L"failed load sound data. path=%s", filePath.c_str());
		}

		// 追加
		_impl->_audioDataList.emplace(name, move(audioData));
		return true;
	}

	bool AudioController::UnLoadSound(const string& name) const {

		if (_impl->_audioDataList.count(name) <= 0) {
			printf("Sound data is not found. key=%s", name.c_str());
			return false;
		}

		// 削除 (shared_ptrなので開放される予定)
		_impl->_audioDataList.erase(name);
		return true;
	}


	bool AudioController::SetMainSound(const string& name) const {


		if (_impl->_audioDataList.count(name) <= 0) {
			printf("Sound data is not found. key=%s", name.c_str());
			return false;
		}

		auto data = _impl->_audioDataList[name];
		auto&& target = _impl->_audioPlayerList["Main"];
		// データをセット
		return FAILED(target->SetAudioData(data));
	}

	float AudioController::GetVolume() const {

		auto&& target = _impl->_audioPlayerList["Main"];
		return target->GetVolume();
	}

	void AudioController::SetVolume(float volume) const {
		
		auto&& target = _impl->_audioPlayerList["Main"];
		target->SetVolume(volume);
	}


	bool AudioController::PlayMainSound() const {

		auto&& target = _impl->_audioPlayerList["Main"];
		// 再生
		return FAILED(target->Play());
	}

	bool AudioController::StopMainSound() const {

		auto&& target = _impl->_audioPlayerList["Main"];
		// 停止
		return FAILED(target->Stop());
	}

	bool AudioController::PauseMainSound() const {

		auto&& target = _impl->_audioPlayerList["Main"];
		// ポーズ
		return FAILED(target->Pause());
	}

}