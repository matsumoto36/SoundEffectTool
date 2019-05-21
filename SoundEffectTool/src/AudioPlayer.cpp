#include "stdafx.h"
#include "../include/AudioPlayer.h"

#include "AudioData.h"
#include "AudioPlayer.h"

namespace SoundEffectTool {

	struct AudioPlayer::Impl {
		unique_ptr<AudioLibrary::AudioPlayer, AudioPlayerDeleter> _player;
		UINT32 _samplesPerSec;
	};

	AudioPlayer::AudioPlayer() :
		_impl(make_unique<Impl>()) {
	
		// プレーヤーを作る
		auto player = move(Audio::GetInstance().CreateAudioPlayer());

		// 通知をリンク
		player->OnIsPlayChanged = [&](bool isPlay) { OnIsPlayChanged(isPlay); };
		player->OnVolumeChanged = [&](float volume) { OnVolumeChanged(volume); };

		_impl->_player = move(player);
	}


	AudioPlayer::~AudioPlayer() {}

	bool AudioPlayer::IsPlay() const {
		return _impl->_player->IsPlay();
	}


	bool AudioPlayer::SetSoundData(shared_ptr<AudioData> audioData) {

		if (!audioData) return false;

		// データをセット
		if (FAILED(_impl->_player->SetAudioData(audioData))) {
			return false;
		}

		_impl->_samplesPerSec = audioData->GetFormat().nSamplesPerSec;
		
		_position = 0;
		// 秒に変換
		_length = (float)audioData->GetLength() / _impl->_samplesPerSec;

		return true;
	}

	float AudioPlayer::GetVolume() const {
		return _impl->_player->GetVolume();
	}

	void AudioPlayer::SetVolume(float volume) const {
		_impl->_player->SetVolume(volume);
	}

	void AudioPlayer::Update(float deltaTime) {
		_impl->_player->Update(deltaTime);

		// 秒に変換
		_position = (float)_impl->_player->GetPosition() / _impl->_samplesPerSec;
	}

	bool AudioPlayer::PlayStart(float position) {

		_position = position;
		if (position != 0) {
			auto samples = (UINT32)(_impl->_samplesPerSec * position);
			return SUCCEEDED(_impl->_player->PlayAtPosition(samples));
		}
		else {
			return SUCCEEDED(_impl->_player->Play());
		}
	}

	bool AudioPlayer::Stop() {
		_position = 0;
		return SUCCEEDED(_impl->_player->Stop());
	}

	bool AudioPlayer::Pause() const {
		return SUCCEEDED(_impl->_player->Pause());
	}
}