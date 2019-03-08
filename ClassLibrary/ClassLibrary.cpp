#include "stdafx.h"

#include <Windows.h>
#include "SoundEffectTool.h"
#include "ClassLibrary.h"

namespace SoundEffectTool {

	bool SoundEffectToolHelper::Initialize() {
		return SoundEffectToolManager::Initialize();
	}

	void SoundEffectToolHelper::Final() {
		SoundEffectToolManager::Final();
	}

	const IntPtr SoundEffectToolHelper::CreateDxView(IntPtr windowHandle, IntPtr% backBuffer, int width, int height) {
		auto hwnd = reinterpret_cast<HWND>(windowHandle.ToPointer());
		const void* bb = nullptr;
		auto renderer = SoundEffectToolManager::CreateDxView(hwnd, width, height, &bb);
		backBuffer = IntPtr(const_cast<void*>(bb));
		auto ptr = move(renderer);
		return IntPtr(&ptr);
	}

	void SoundEffectToolHelper::ChangeDrawSize(IntPtr renderer, int width, int height) {
		auto r = reinterpret_cast<Renderer*>(renderer.ToPointer());
		r->ChangeDrawSize(width, height);
	}
}
