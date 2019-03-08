// SoundEffectToolModel.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"

#include "SoundEffectTool.h"
#include "AudioLibrary.h"

namespace SoundEffectTool {

	SoundEffectToolManager* SoundEffectToolManager::_instance;

	SoundEffectToolManager::SoundEffectToolManager()
	{}

	SoundEffectToolManager::~SoundEffectToolManager() {}

	bool SoundEffectToolManager::Initialize() {
		_instance = new SoundEffectToolManager();
		return true;
	}
	void SoundEffectToolManager::Final() {
		if(_instance) delete _instance;
	}

	Renderer& SoundEffectToolManager::CreateDxView(HWND windowHandle, int width, int height, const void** backBuffer) {
		auto renderer = make_unique<Renderer>();
		renderer->Initialize(windowHandle, width, height);
		auto bb = renderer->GetBackBuffer();
		*backBuffer = bb;
		return *renderer;
	}
}