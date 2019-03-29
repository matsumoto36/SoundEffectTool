#pragma once

#include <math.h>
#include <memory>

#include "AudioDefine.h"
#include "AudioData.h"

namespace AudioLibrary {

	// 音声データを再生するクラス
	class AUDIOLIBRARY_API AudioPlayer {

	private:
			
		bool _isPlay;		// 再生しているか
		float _volume;		// 現在の音量

		struct Impl;
		unique_ptr<Impl> _impl;

	public:
		AudioPlayer(IXAudio2& xAudio2, float volume = 1.0f);
		~AudioPlayer();

		// コピーは禁止するが、ムーブは許可する
		AudioPlayer(const AudioPlayer&) = delete;
		AudioPlayer(AudioPlayer&&) noexcept;
		AudioPlayer& operator=(const AudioPlayer&) = delete;

		// 現在音声を再生しているか
		bool IsPlay() const {
			return _isPlay;
		}

		float GetVolume() const {
			return _volume;
		}

		void SetVolume(float volume);

		// 再生システムの情報を更新する
		void Update();

		// プレイヤーに音声データを入力する
		HRESULT SetAudioData(const shared_ptr<AudioData>& audioData);

		// プレイヤーから音声データを削除する
		void UnSetAudioData();

		// 音を再生する
		HRESULT Play() const;

		// 音を止める
		HRESULT Stop() const;

		// 音をポーズする
		HRESULT Pause() const;
	};

	// プレイヤーのデリーター
	struct AudioPlayerDeleter {
		void operator()(AudioPlayer* player) const {
			// 再生中なら止める
			player->Stop();

			// リソースの開放
			player->UnSetAudioData();

			delete player;
		}
	};
}

