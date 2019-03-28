#pragma once

#include <memory>

#include "AudioDefine.h"
#include "AudioData.h"

namespace AudioLibrary {

	// 音声データを再生するクラス
	class AUDIOLIBRARY_API AudioPlayer {

	public:

	private:

		IXAudio2& _xAudio2;
		
		shared_ptr<AudioData> _audioData;	// 再生する音声データ
		IXAudio2SourceVoice* _sourceVoice;	// ソースボイス

	public:
		AudioPlayer(IXAudio2& xAudio2);
		~AudioPlayer();

		// コピーは禁止するが、ムーブは許可する
		AudioPlayer(const AudioPlayer&) = delete;
		AudioPlayer(AudioPlayer&&) noexcept = default;
		AudioPlayer& operator=(const AudioPlayer&) = delete;

		// プレイヤーに音声データを入力する
		HRESULT SetAudioData(const shared_ptr<AudioData>& audioData);

		// プレイヤーから音声データを削除する
		void UnSetAudioData();

		// 現在音声を再生しているか
		bool IsPlay();

		// 音を再生する
		HRESULT Play();

		// 音を止める
		HRESULT Stop();

		// 音をポーズする
		HRESULT Pause();
	};

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

