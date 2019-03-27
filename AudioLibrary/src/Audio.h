#pragma once

#include <memory>
#include <string>

#include "AudioDefine.h"
#include "AudioLoader.h"
#include "AudioPlayer.h"
#include "AudioResource.h"

namespace AudioLibrary {

	using namespace std;

	// サウンドシステムを管理するクラス
	class AUDIOLIBRARY_API Audio final sealed {

		static Audio& _instance;					// singleton
		static bool _isInitialized;					// 初期化が完了しているか

		IXAudio2* _xAudio2;							// XAudio2のシステム
		IXAudio2MasteringVoice* _masteringVoice;	// マスターボイス

		Audio() {}
		~Audio() { Finalize(); }

	public:

		static Audio& GetInstance();

		void Initialize();
		void Finalize();

		// プレーヤーを作成する
		AudioPlayerHandler CreateAudioPlayer();

		// 音声ファイルを読み込む
		shared_ptr<AudioData> LoadAudioData(const wstring& filePath);

	};
}