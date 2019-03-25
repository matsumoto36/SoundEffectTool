#pragma once

#include <memory>
#include "AudioData.h"

namespace {

	// ソースボイスのデリータ
	struct SourceVoiceDeleter {
		void operator()(IXAudio2SourceVoice* voice) const {
			voice->DestroyVoice();
		}
	};

}

namespace AudioLibrary {

	// 音声データを再生するクラス
	class AUDIOLIBRARY_API AudioPlayer {

	public:

	private:

		IXAudio2& _xAudio2;
		
		shared_ptr<AudioData> _audioData;									// 再生する音声データ
		unique_ptr<IXAudio2SourceVoice, SourceVoiceDeleter> _sourceVoice;	// ソースボイス

	public:
		AudioPlayer(const unique_ptr<IXAudio2>& xAudio2);
		~AudioPlayer();

		// コピーは禁止するが、ムーブは許可する
		AudioPlayer(const AudioPlayer&) = delete;
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
}

