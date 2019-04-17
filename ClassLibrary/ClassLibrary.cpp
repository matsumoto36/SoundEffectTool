#include "stdafx.h"

#include <Windows.h>

#define SOUNDEFFECTTOOL_API __declspec(dllimport)
#include "SoundEffectTool.h"

#include <vcclr.h>
#include <msclr/marshal_cppstd.h>

#include "ClassLibrary.h"
#include "Extensions.h"

namespace {
	using namespace SoundEffectTool;

	// 通知をリンクする
	void LinkEvent(SoundEffectToolVM^ manager) {

		gcroot<SoundEffectToolVM^> m = manager;

		m->_audioController.OnIsPlayChanged = [m](bool isPlay) {
			m->CallOnAudioIsPlayChanged(isPlay);
		};

		m->_audioController.OnVolumeChanged = [m](float volume) {
			m->CallOnAudioVolumeChanged(volume);
		};
	}

}

namespace SoundEffectTool {

	SoundEffectToolVM::SoundEffectToolVM() :
		_manager(new SoundEffectToolManager()),
		_audioController(_manager->GetAudioController()) {
		
		// 通知をリンク
		LinkEvent(this);
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
		renderer.DrawWave();
	}

	void SoundEffectToolVM::UpdateAudio() {
		auto isPlay = _audioController.IsPlay();
		_audioController.Update();
	}

	float SoundEffectToolVM::GetVolume() {
		return _audioController.GetVolume();
	}

	void SoundEffectToolVM::SetVolume(float volume) {
		_audioController.SetVolume(volume);
	}

	bool SoundEffectToolVM::IsPlay() {
		return _audioController.IsPlay();
	}

	bool SoundEffectToolVM::LoadSound(String^ filePath, String^ name) {
		return _audioController.LoadSound(ToStdWString(filePath), ToStdString(name));
	}

	bool SoundEffectToolVM::UnLoadSound(String^ name) {
		return _audioController.UnLoadSound(ToStdString(name));
	}

	bool SoundEffectToolVM::SetMainSound(String^ name) {
		return _audioController.SetMainSound(ToStdString(name));
	}

	bool SoundEffectToolVM::PlayMainSound() {
		return _audioController.PlayMainSound();
	}

	bool SoundEffectToolVM::StopMainSound() {
		return _audioController.StopMainSound();
	}

	bool SoundEffectToolVM::PauseMainSound() {
		return _audioController.PauseMainSound();
	}
}
