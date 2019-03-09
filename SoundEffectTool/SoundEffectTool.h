#pragma once

#define SOUNDEFFECTTOOL_API __declspec(dllexport)

#include <vector>
#include <memory>
#include <map>

#include "Renderer.h"

namespace AudioLibrary {
	class Audio;
}

using namespace std;
using namespace AudioLibrary;

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API SoundEffectToolManager final sealed {

		map<string, unique_ptr<Renderer>> _rendererList;

	public:

		// コピーは禁止するが、ムーブは許可する
		SoundEffectToolManager(const SoundEffectToolManager&) = delete;
		SoundEffectToolManager& operator=(const SoundEffectToolManager&) = delete;

		SoundEffectToolManager();
		~SoundEffectToolManager();
		
		// ウィンドウの生成
		void CreateDxView(HWND windowHandle, string rendererName, int width, int height);

		Renderer& GetRenderer(string rendererName);
		
	private:

	};
}
