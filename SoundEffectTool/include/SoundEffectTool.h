#pragma once

// unique_ptr使用につき
#pragma warning(disable:4251)

#include <vector>
#include <memory>
#include <map>

#include "SoundEffectToolDefine.h"
#include "Renderer.h"
#include "AudioController.h"

using namespace std;

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API SoundEffectToolManager final sealed {

		map<string, unique_ptr<Renderer>> _rendererList;
		unique_ptr<AudioController> _audioController;

	public:

		// コピーは禁止するが、ムーブは許可する
		SoundEffectToolManager(const SoundEffectToolManager&) = delete;
		SoundEffectToolManager& operator=(const SoundEffectToolManager&) = delete;

		SoundEffectToolManager();
		~SoundEffectToolManager();
		
		// ウィンドウの生成
		void CreateDxView(HWND windowHandle, string& rendererName, int width, int height);

		const Renderer& GetRenderer(string& rendererName);

		AudioController& GetAudioController() const;

	private:

	};
}
