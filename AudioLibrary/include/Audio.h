#pragma once

#include <memory>
#include <string>

#include "AudioLoader.h"
#include "AudioPlayer.h"

namespace AudioLibrary {

	using namespace std;

	// サウンドシステムを管理するクラス
	class AUDIOLIBRARY_API Audio final sealed {

		static Audio& _instance;					// singleton
		static bool _isInitialized;					// 初期化が完了しているか

		class Impl;
		unique_ptr<Impl> _impl;

		Audio();
		~Audio() { Finalize(); }

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