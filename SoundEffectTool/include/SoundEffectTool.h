#pragma once

// unique_ptr使用につき
#pragma warning(disable:4251)

#include <vector>
#include <memory>
#include <map>

#include "SoundEffectToolDefine.h"
#include "Renderer.h"

using namespace std;
using namespace AudioLibrary;

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API SoundEffectToolManager final sealed {

		map<string, unique_ptr<Renderer>> _rendererList;

		class Impl;
		unique_ptr<Impl> _impl;

	public:

		// コピーは禁止するが、ムーブは許可する
		SoundEffectToolManager(const SoundEffectToolManager&) = delete;
		SoundEffectToolManager& operator=(const SoundEffectToolManager&) = delete;

		SoundEffectToolManager();
		~SoundEffectToolManager();
		
		// ウィンドウの生成
		void CreateDxView(HWND windowHandle, string& rendererName, int width, int height);

		const Renderer& GetRenderer(string& rendererName);

		// 音声をロードする
		bool LoadSound(const wstring& filePath, const string& name) const;

		// 音声をロードする
		bool UnLoadSound(const string& name) const;

		// 音声を再生する
		bool PlaySound(const string& name) const;

	private:

	};
}
