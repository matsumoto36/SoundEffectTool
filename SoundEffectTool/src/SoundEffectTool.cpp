// SoundEffectToolModel.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"

#include "../include/SoundEffectTool.h"

namespace SoundEffectTool {

	SoundEffectToolManager::SoundEffectToolManager() :
		_audioController(make_unique<AudioController>()) { }

	SoundEffectToolManager::~SoundEffectToolManager() { }

	void SoundEffectToolManager::CreateDxView(HWND windowHandle, string& rendererName, int width, int height) {
		auto renderer = make_unique<Renderer>();
		renderer->Initialize(windowHandle, width, height);
		_rendererList.emplace(rendererName, move(renderer));
	}

	const Renderer& SoundEffectToolManager::GetRenderer(string& rendererName) {
		return *_rendererList[rendererName];
	}

	AudioController& SoundEffectToolManager::GetAudioController() const {
		return *_audioController;
	}
}