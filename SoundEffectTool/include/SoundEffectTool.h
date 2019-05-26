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
		
		const int DefaultWaveScaleY = 100;
		const float DefaultPixelsPerSec = 64;
		
		map<string, unique_ptr<Renderer>> _rendererList;
		unique_ptr<AudioController> _audioController;

	public:

		// コピーは禁止するが、ムーブは許可する
		SoundEffectToolManager(const SoundEffectToolManager&) = delete;
		SoundEffectToolManager& operator=(const SoundEffectToolManager&) = delete;

		SoundEffectToolManager();
		~SoundEffectToolManager();
		
		// ウィンドウの生成
		void CreateDxView(HWND windowHandle, string& rendererName, PointInt size);

		const unique_ptr<Renderer>& GetRenderer(string& rendererName);

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
