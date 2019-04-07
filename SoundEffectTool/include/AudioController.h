#pragma once

#include <memory>
#include <functional>

#include "SoundEffectToolDefine.h"

using namespace std;
using namespace AudioLibrary;

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API AudioController {

	public:
		function<void(bool)> OnIsPlayChanged;	// 再生状態が変化したときに呼ばれる
		function<void(float)> OnVolumeChanged;	// 音量が変化したときに呼ばれる

	private:
		struct Impl;
		unique_ptr<Impl> _impl;

	public:
		AudioController();
		~AudioController();

		// 再生中かどうかを取得する
		bool IsPlay() const;

		// 情報を更新する
		void Update() const;

		// 音声をロードする
		bool LoadSound(const wstring& filePath, const string& name) const;

		// 音声を解放する
		bool UnLoadSound(const string& name) const;

		// 音声を再生可能状態にする
		bool SetMainSound(const string& name) const;

		// 音量を取得する
		float GetVolume() const;

		// 音量を設定する
		void SetVolume(float volume) const;

		// 音声を再生する
		bool PlayMainSound() const;

		// 音声を停止する
		bool StopMainSound() const;
		
		// 音声を一時停止する
		bool PauseMainSound() const;

	};

}