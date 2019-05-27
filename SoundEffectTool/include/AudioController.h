#pragma once

#include <map>
#include <memory>
#include <functional>

#include "AudioPlayer.h"
#include "SoundEffectToolDefine.h"

using namespace std;
using namespace AudioLibrary;

namespace SoundEffectTool {

	// ライブラリのサウンドシステムを制御するクラス
	class SOUNDEFFECTTOOL_API AudioController {

	private:

		map<string, shared_ptr<AudioPlayer>> _audioPlayerList;	// 生成したプレイヤーのテーブル
		map<string, shared_ptr<AudioData>> _audioDataList;		// 読み込んだ音声データのテーブル

	public:
		// コピーは禁止するが、ムーブは許可する
		AudioController(const AudioController&) = delete;
		AudioController& operator=(const AudioController&) = delete;

		AudioController();
		~AudioController();

		// プレイヤーを作成する(keyをアクセスキーとして登録)
		bool CreateAudioPlayer(const string& key);

		// プレイヤーを取得する
		shared_ptr<AudioPlayer> GetAudioPlayer(const string& key);

		// 音声データを取得する
		shared_ptr<AudioData> GetAudioData(const string& key);

		// 情報を更新する
		void Update(float deltaTime) const;

		// 音声をロードする(keyをアクセスキーとして登録)
		bool LoadSound(const wstring& filePath, const string& key);

		// 音声を解放する
		bool UnLoadSound(const string& key);

	private:

		// プレイヤーが存在しているか
		bool IsValidAudioPlayer(const string& key) const;

		// データが存在しているか
		bool IsValidAudioData(const string& key) const;

	};

}