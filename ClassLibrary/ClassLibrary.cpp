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

		auto player = m->_audioController.GetAudioPlayer("Main");

		player->OnIsPlayChanged = [m](bool isPlay) {
			m->CallOnAudioIsPlayChanged(isPlay);
		};

		player->OnVolumeChanged = [m](float volume) {
			m->CallOnAudioVolumeChanged(volume);
		};
	}

}

namespace SoundEffectTool {

	SoundEffectToolVM::SoundEffectToolVM() :
		_manager(new SoundEffectToolManager()),
		_audioController(_manager->GetAudioController()) {
		
		// メインで扱うプレイヤーを作成
		_audioController.CreateAudioPlayer("Main");

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
		auto&& renderer = _manager->GetRenderer(ToStdString(windowName));
		if (!renderer) 
			return IntPtr(nullptr);
		return IntPtr(const_cast<void*>(renderer->GetBackBuffer()));
	}

	bool SoundEffectToolVM::ChangeDrawSize(String^ windowName, int width, int height) {
		auto&& renderer = _manager->GetRenderer(ToStdString(windowName));
		if (!renderer) return false;

		renderer->ChangeDrawSize(width, height);
		return true;
	}

	bool SoundEffectToolVM::Draw(String^ windowName) {
		auto&& renderer = _manager->GetRenderer(ToStdString(windowName));
		if (!renderer) 
			return false;

		renderer->DrawWave();
		return true;
	}

	void SoundEffectToolVM::UpdateAudio(float deltaTime) {
		_audioController.Update(deltaTime);
	}

	bool SoundEffectToolVM::LoadSound(String^ filePath, String^ key) {
		return _audioController.LoadSound(ToStdWString(filePath), ToStdString(key));
	}

	bool SoundEffectToolVM::UnLoadSound(String^ key) {
		return _audioController.UnLoadSound(ToStdString(key));
	}

	bool SoundEffectToolVM::SetMainSound(String^ key) {
		return _manager->SetSoundData(ToStdString("Main"), ToStdString(key));
	}

	bool SoundEffectToolVM::SetWaveData(String^ windowName, String^ key) {
		return _manager->SetWaveData(ToStdString(windowName), ToStdString(key));
	}

	bool SoundEffectToolVM::IsPlay() {
		return _audioController.GetAudioPlayer("Main")->IsPlay();
	}

	float SoundEffectToolVM::GetVolume() {
		return _audioController.GetAudioPlayer("Main")->GetVolume();
	}

	void SoundEffectToolVM::SetVolume(float volume) {
		_audioController.GetAudioPlayer("Main")->SetVolume(volume);
	}

	float SoundEffectToolVM::GetMainPlayerPosition() {
		return _audioController.GetAudioPlayer("Main")->GetPlayPosition();
	}

	float SoundEffectToolVM::GetSoundFileLength() {
		return _audioController.GetAudioPlayer("Main")->GetFileLength();
	}

	bool SoundEffectToolVM::PlayMainSound() {
		return _audioController.GetAudioPlayer("Main")->PlayStart();
	}

	bool SoundEffectToolVM::PlayMainSoundAtPosition(float position) {
		return _audioController.GetAudioPlayer("Main")->PlayStart(position);
	}

	bool SoundEffectToolVM::StopMainSound() {
		return _audioController.GetAudioPlayer("Main")->Stop();
	}

	bool SoundEffectToolVM::PauseMainSound() {
		return _audioController.GetAudioPlayer("Main")->Pause();
	}
}
