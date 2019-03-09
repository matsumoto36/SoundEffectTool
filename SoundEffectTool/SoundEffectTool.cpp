// SoundEffectToolModel.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"

#include "SoundEffectTool.h"
#include "AudioLibrary.h"

namespace SoundEffectTool {

	SoundEffectToolManager::SoundEffectToolManager()
	{}

	SoundEffectToolManager::~SoundEffectToolManager() {}

	void SoundEffectToolManager::CreateDxView(HWND windowHandle, string rendererName, int width, int height) {
		auto renderer = make_unique<Renderer>();
		renderer->Initialize(windowHandle, width, height);
		_rendererList.emplace(rendererName, move(renderer));
	}

	Renderer& SoundEffectToolManager::GetRenderer(string rendererName) {
		return *_rendererList[rendererName];
	}
}