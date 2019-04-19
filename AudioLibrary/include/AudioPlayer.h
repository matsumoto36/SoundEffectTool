#pragma once

#include <math.h>
#include <memory>
#include <functional>

#include "AudioDefine.h"
#include "AudioPlayerStatus.h"
#include "AudioData.h"

namespace AudioLibrary {

	// 音声データを再生するクラス
	class AUDIOLIBRARY_API AudioPlayer {

	public:
		function<void(bool)> OnIsPlayChanged;			// 再生状態が変化したときに呼ばれる
		function<void(float)> OnVolumeChanged;			// 音量が変化したときに呼ばれる

	private:
		AudioPlayerStatus _status;	// 再生状態
		float _volume;				// 現在の音量
		UINT32 _position;			// 再生位置	

		class VoiceCallback;		// XAudio2のコールバック

		struct Impl;
		unique_ptr<Impl> _impl;

	public:
		AudioPlayer(IXAudio2& xAudio2, float volume = 1.0f);
		~AudioPlayer();

		// コピーは禁止するが、ムーブは許可する
		AudioPlayer(const AudioPlayer&) = delete;
		AudioPlayer& operator=(const AudioPlayer&) = delete;

		// 現在音声を再生しているか
		bool IsPlay() const {
			return _status == AudioPlayerStatus::Play;
		}

		float GetVolume() const {
			return _volume;
		}

		void SetVolume(float volume);

		// 再生位置を取得
		UINT32 GetPosition() const {
			return _position;
		}

		const shared_ptr<AudioData>& GetAudioData() const;

		// プレイヤーに音声データを入力する
		HRESULT SetAudioData(const shared_ptr<AudioData>& audioData);

		// プレイヤーから音声データを削除する
		void UnSetAudioData();

		// 再生システムの情報を更新する
		void Update();

		// 音を再生する
		HRESULT Play();

		// 音を位置指定して再生する
		HRESULT PlayAtPosition(UINT32 samples);

		// 音を止める
		HRESULT Stop();

		// 音をポーズする
		HRESULT Pause();

	private:

		// 再生の状態を設定する
		void SetPlayerStatus(AudioPlayerStatus status);

		// データが整っているか調べる
		HRESULT CheckValidData() const;

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

