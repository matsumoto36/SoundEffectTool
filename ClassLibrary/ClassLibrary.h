#pragma once

using namespace System;
using namespace System::Windows;

namespace SoundEffectTool {

	// GUIとシステムをつなぐ橋渡し的存在
	public ref class SoundEffectToolVM {

	internal:
		class SoundEffectToolManager* _manager;
		AudioController& _audioController;

		// コールバックバインド用
		void CallOnAudioIsPlayChanged(bool isPlay) { OnAudioIsPlayChanged(isPlay); }
		void CallOnAudioPlayingEnd() { OnAudioPlayingEnd(); }
		void CallOnAudioVolumeChanged(float volume) { OnAudioVolumeChanged(volume); }

	private:

	public:

		// 再生の状態が変化した場合に呼ばれる
		event Action<bool>^ OnAudioIsPlayChanged;

		// 最後まで再生されたときに呼ばれる
		event Action^ OnAudioPlayingEnd;

		// 音量が変化した場合に呼ばれる
		event Action<float>^ OnAudioVolumeChanged;

		SoundEffectToolVM();
		~SoundEffectToolVM();
		!SoundEffectToolVM();
		
		// ウィンドウの生成
		void CreateDxView(IntPtr windowHandle, String^ windowName, Size size);

		// 対象のウィンドウのバックバッファを取得する
		const IntPtr GetBackBuffer(String^ windowName);

		// 描画サイズを変更する
		bool ChangeDrawSize(String^ windowName, PointInt size);

		// 描画サイズを返す
		Size GetDrawSize(String^ windowName);

		// 描画する
		bool Draw(String^ windowName);

		// 音声の情報を更新する
		void UpdateAudio(float deltaTime);

		// 音声をロードする(keyをアクセスキーとして登録)
		bool LoadSound(String^ filePath, String^ key);

		// 音声を解放する
		bool UnLoadSound(String^ key);

		// 音声を再生可能状態にする
		bool SetMainSound(String^ key);

		// 音声を描画にセット
		bool SetWaveData(String^ windowName, String^ key);

		// 再生しているかを取得する
		bool IsPlay();

		// 音量を取得する
		float GetVolume();

		// 音量を設定する
		void SetVolume(float volume);

		// 再生位置を取得する
		float GetMainPlayerPosition();

		// 音声の長さを取得する
		float GetSoundFileLength();

		// 音声を再生する
		bool PlayMainSound();

		// 音声を途中から再生する
		bool PlayMainSoundAtPosition(float position);

		// 音声を停止する
		bool StopMainSound();

		// 音声を一時停止する
		bool PauseMainSound();

	};
}
