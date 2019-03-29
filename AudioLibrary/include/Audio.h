#pragma once

#include <memory>
#include <string>

#include "AudioDefine.h"
#include "AudioLoader.h"
#include "AudioPlayer.h"

using namespace std;

namespace AudioLibrary {

	// デリーター
	struct XAudio2Deleter {
		void operator()(IXAudio2* xaudio2) const;
	};

	struct MasteringVoiceDeleter {
		void operator()(IXAudio2MasteringVoice* masteringVoice) const;
	};

	// サウンドシステムを管理するクラス
	class AUDIOLIBRARY_API Audio final sealed {

		static Audio& _instance;					// singleton
		static bool _isInitialized;					// 初期化が完了しているか

		unique_ptr<IXAudio2, XAudio2Deleter> _xAudio2;								// XAudio2のシステム
		unique_ptr<IXAudio2MasteringVoice, MasteringVoiceDeleter> _masteringVoice;	// マスターボイス

		Audio();
		~Audio();

	public:

		static Audio& GetInstance();

		void Initialize();
		void Finalize();

		// プレーヤーを作成する
		unique_ptr<AudioPlayer, AudioPlayerDeleter> CreateAudioPlayer();

		// 音声ファイルを読み込む
		shared_ptr<AudioData> LoadAudioData(const wstring& filePath);

	};
}