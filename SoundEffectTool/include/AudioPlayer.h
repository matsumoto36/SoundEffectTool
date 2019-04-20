#pragma once

#include <memory>

#include "SoundEffectToolDefine.h"

using namespace std;
using namespace AudioLibrary;

namespace SoundEffectTool {

	// ライブラリのプレイヤーをラップしたもの
	class SOUNDEFFECTTOOL_API AudioPlayer {

	public:
		function<void(bool)> OnIsPlayChanged;	// 再生状態が変化したときに呼ばれる
		function<void(float)> OnVolumeChanged;	// 音量が変化したときに呼ばれる

	private:

		float _position;	// 再生位置(s)
		float _length;		// ファイルの再生時間(s)

		struct Impl;
		unique_ptr<Impl> _impl;

	public:

		// コピーは禁止するが、ムーブは許可する
		AudioPlayer(const AudioPlayer&) = delete;
		AudioPlayer& operator=(const AudioPlayer&) = delete;

		AudioPlayer();
		~AudioPlayer();

		// 再生中かどうかを取得する
		bool IsPlay() const;

		// 現在の再生位置を取得する
		float GetPlayPosition() const {
			return _position;
		}

		// ファイルの再生時間を取得する
		float GetFileLength() const {
			return _length;
		}

		// 音声を再生可能状態にする
		bool SetSoundData(shared_ptr<AudioData> audioData);

		// 音量を取得する
		float GetVolume() const;

		// 音量を設定する
		void SetVolume(float volume) const;

		// プレイヤーの状態を更新する
		void Update(float deltaTime);

		// 音声を再生する
		bool PlayStart(float position = 0);

		// 音声を停止する
		bool Stop();

		// 音声を一時停止する
		bool Pause() const;
	};

}