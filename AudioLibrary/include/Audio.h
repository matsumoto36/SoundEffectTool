#pragma once

// unique_ptr使用につき
#pragma warning(disable:4251)

#include <memory>
#include <string>

#include "AudioDefine.h"
#include "AudioLoader.h"
#include "AudioPlayer.h"

namespace AudioLibrary {

	using namespace std;

	// サウンドシステムを管理するクラス
	class AUDIOLIBRARY_API Audio final sealed {

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