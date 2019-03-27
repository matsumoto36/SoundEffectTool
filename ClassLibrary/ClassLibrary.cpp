#include "stdafx.h"

#include <Windows.h>
#include "SoundEffectTool.h"
#include "ClassLibrary.h"
#include "Extensions.h"

namespace SoundEffectTool {

	SoundEffectToolVM::SoundEffectToolVM()
		: _manager(new SoundEffectToolManager()) {
	
	}

	SoundEffectToolVM::~SoundEffectToolVM() {
		this->!SoundEffectToolVM();
	}

	SoundEffectToolVM::!SoundEffectToolVM() {
		delete _manager;
	}

	void SoundEffectToolVM::CreateDxView(IntPtr windowHandle, String^ windowName, int width, int height) {
		auto hwnd = reinterpret_cast<HWND>(windowHandle.ToPointer());
		_manager->CreateDxView(hwnd, ToStdString(windowName), width, height);
	}

	const IntPtr SoundEffectToolVM::GetBackBuffer(String^ windowName) {
		auto renderer = _manager->GetRenderer(ToStdString(windowName));
		return IntPtr(const_cast<void*>(renderer.GetBackBuffer()));
	}

	void SoundEffectToolVM::ChangeDrawSize(String^ windowName, int width, int height) {
		auto renderer = _manager->GetRenderer(ToStdString(windowName));
		renderer.ChangeDrawSize(width, height);
	}

	void SoundEffectToolVM::Draw(String^ windowName) {
		auto renderer = _manager->GetRenderer(ToStdString(windowName));
		renderer.Draw();
	}

	void SoundEffectToolVM::PlaySoundFromFile(String^ fileName) {
		_manager->PlaySoundFromFile(ToStdWString(fileName));
	}

}
