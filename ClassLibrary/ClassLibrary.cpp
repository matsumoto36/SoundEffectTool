#include "stdafx.h"

#include <Windows.h>
#include "SoundEffectTool.h"
#include "ClassLibrary.h"

using namespace SoundEffectTool;

namespace SoundEffectTool {

	bool SoundEffectToolHelper::Initialize() {
		return SoundEffectToolManager::Initialize();
	}
	void SoundEffectToolHelper::Final() {
		SoundEffectToolManager::Final();
	}

	const IntPtr SoundEffectToolHelper::CreateDxView(IntPtr windowHandle) {
		auto hwnd = reinterpret_cast<HWND>(windowHandle.ToPointer());
		auto ptr = SoundEffectToolManager::CreateDxView(hwnd);
		return IntPtr(&ptr);
	}
}
