#pragma once

// unique_ptr使用につき
#pragma warning(disable:4251)

#include <string>

#include "SoundEffectToolDefine.h"
#include "Renderer.h"

using namespace std;

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API SoundEffectToolManager final sealed {

	public:

		// コピーは禁止するが、ムーブは許可する
		SoundEffectToolManager(const SoundEffectToolManager&) = delete;
		SoundEffectToolManager& operator=(const SoundEffectToolManager&) = delete;

		SoundEffectToolManager();
		~SoundEffectToolManager();

		// ウィンドウの生成
		void CreateDxView(HWND windowHandle, string& rendererName, int width, int height);

		const Renderer& GetRenderer(string& rendererName);

		// 音声を再生
		void PlaySoundFromFile(const wstring& filePath) const;

	private:

	};
}
