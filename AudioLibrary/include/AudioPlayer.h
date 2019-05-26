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
		function<void(void)> OnPlayingEnd;				// 最後まで再生されたときに呼ばれる
		function<void(float)> OnVolumeChanged;			// 音量が変化したときに呼ばれる

	private:
		AudioPlayerStatus _status;		// 再生状態
		float _volume;					// 現在の音量
		uint64_t _position;				// 再生位置(サンプル数)
		uint64_t _seekData;				// 再生位置(バイト数)

		float _targetTime;				// フェードを行う時間
		float _fading = 0;				// フェードの経過時間
		float _fadeStartVolume;			// フェード開始時の音量
		float _targetVolume;			// フェード中に目指す音量
		function<void()> _fadeCallback;	// フェードが完了したときに実行される

		class VoiceCallback;			// XAudio2のコールバック

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
		UINT64 GetPosition() const {
			return _position;
		}

		const shared_ptr<AudioData>& GetAudioData() const;

		// プレイヤーに音声データを入力する
		HRESULT SetAudioData(const shared_ptr<AudioData>& audioData);

		// プレイヤーから音声データを削除する
		void UnSetAudioData();

		// 再生システムの情報を更新する
		void Update(float deltaTime);

		// 音を再生する
		HRESULT Play(uint32_t samples);

		// 音を止める
		HRESULT Stop();

		// 音をポーズする
		HRESULT Pause();

	private:

		// 再生の状態を設定する
		void SetPlayerStatus(AudioPlayerStatus status);

		// データが整っているか調べる
		HRESULT CheckValidData() const;

		void AddBuffer(uint32_t requredBytes);

		// 内部で使用するためのフェード
		void SetFade(float targetVolume, float targetTime, function<void()> callback = nullptr);

		// フェード更新
		void UpdateFade(float deltaTime);

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

