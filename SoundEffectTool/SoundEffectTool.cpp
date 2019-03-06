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

	// ウィンドウの生成
	// 戻り値はバックバッファ
	const void* SoundEffectToolManager::CreateDxView(HWND windowHandle) {
		auto renderer = make_unique<Renderer>();
		renderer->Initialize(windowHandle);
		//_instance->_rendererList.emplace_back(move(renderer));
		_instance->_renderer = move(renderer);
		return renderer.get()->GetBackBuffer();
	}
}