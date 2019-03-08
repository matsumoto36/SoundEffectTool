#pragma once

#define SOUNDEFFECTTOOL_API __declspec(dllexport)

#include <vector>
#include <memory>

#include "Renderer.h"
#include "Extensions.h"

namespace AudioLibrary {
	class Audio;
}

using namespace std;
using namespace AudioLibrary;

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API SoundEffectToolManager final sealed {

		static SoundEffectToolManager* _instance;

	public:

		static bool Initialize();
		static void Final();

		// ウィンドウの生成
		// 戻り値はバックバッファ
		static Renderer& CreateDxView(HWND, int, int, const void**);
		
	private:
		
		SoundEffectToolManager();
		~SoundEffectToolManager();

	};
}
