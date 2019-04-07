#pragma once

using namespace System;

namespace SoundEffectTool {

	public ref class SoundEffectToolVM {

	internal:
		class SoundEffectToolManager* _manager;
		AudioController& _audioController;

		// コールバックバインド用
		void CallOnAudioIsPlayChanged(bool isPlay) { OnAudioIsPlayChanged(isPlay); }
		void CallOnAudioVolumeChanged(float volume) { OnAudioVolumeChanged(volume); }

	private:

	public:

		// 再生の状態が変化した場合に呼ばれる
		event Action<bool>^ OnAudioIsPlayChanged;

		// 音量が変化した場合に呼ばれる
		event Action<float>^ OnAudioVolumeChanged;

		SoundEffectToolVM();
		~SoundEffectToolVM();
		!SoundEffectToolVM();
		
		// ウィンドウの生成
		void CreateDxView(IntPtr windowHandle, String^ windowName, int width, int height);

		// 対象のウィンドウのバックバッファを取得する
		const IntPtr GetBackBuffer(String^ windowName);

		// 描画サイズを変更する
		void ChangeDrawSize(String^ windowName, int width, int height);

		// 描画する
		void Draw(String^ windowName);

		// 音声の情報を更新する
		void UpdateAudio();

		// 音声をロードする
		bool LoadSound(String^ filePath, String^ name);

		// 音声を解放する
		bool UnLoadSound(String^ name);

		// 音声を再生可能状態にする
		bool SetMainSound(String^ name);

		// 再生しているかを取得する
		bool IsPlay();

		// 音量を取得する
		float GetVolume();

		// 音量を設定する
		void SetVolume(float volume);

		// 音声を再生する
		bool PlayMainSound();

		// 音声を停止する
		bool StopMainSound();

		// 音声を一時停止する
		bool PauseMainSound();

	};
}
