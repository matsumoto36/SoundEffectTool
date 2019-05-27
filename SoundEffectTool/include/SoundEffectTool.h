#pragma once

// unique_ptr使用につき
#pragma warning(disable:4251)

#include <vector>
#include <memory>
#include <map>

#include "Renderer.h"
#include "AudioController.h"
#include "SoundEffectToolDefine.h"

using namespace std;

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API SoundEffectToolManager final sealed {
		
		map<string, unique_ptr<Renderer>> _rendererList;	// 描画ウィンドウのテーブル
		unique_ptr<AudioController> _audioController;		// サウンドコントローラー

	public:

		const int DefaultWaveHeight = 100;					// 波形の描画の高さ
		const float DefaultPixelsPerSec = 64;				// 1秒間のデータを何ピクセルで表すか

		// コピーは禁止するが、ムーブは許可する
		SoundEffectToolManager(const SoundEffectToolManager&) = delete;
		SoundEffectToolManager& operator=(const SoundEffectToolManager&) = delete;

		SoundEffectToolManager();
		~SoundEffectToolManager();
		
		// ウィンドウの生成
		void CreateDxView(HWND windowHandle, string& rendererName, PointInt size);

		// 描画を返す
		const unique_ptr<Renderer>& GetRenderer(string& rendererName);

		// サウンドコントローラーを返す
		AudioController& GetAudioController() const;

		// 音声データをプレイヤーにセット
		bool SetSoundData(string& playerKey, string& audioDataKey);

		// 波形データを描画にセット
		bool SetWaveData(string& rendererName, string& key);

		// 波形の表示をスケーリングする
		bool CalcWaveRenderingScale(string& rendererName, float pixelsPerSec);

	private:

	};
}
