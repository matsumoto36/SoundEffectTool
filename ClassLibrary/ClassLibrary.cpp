#include "stdafx.h"

#include <Windows.h>

#define SOUNDEFFECTTOOL_API __declspec(dllimport)
#include "SoundEffectTool.h"

#include <vcclr.h>
#include <msclr/marshal_cppstd.h>

#include "ClassLibrary.h"
#include "Extensions.h"

namespace SoundEffectTool {
	string MainSoundKey = "Main";
}

namespace {
	using namespace SoundEffectTool;

	// 通知をリンクする
	void LinkEvent(SoundEffectToolVM^ manager) {

		gcroot<SoundEffectToolVM^> m = manager;

		auto player = m->_audioController.GetAudioPlayer(MainSoundKey);

		player->OnIsPlayChanged = [m](bool isPlay) {
			m->CallOnAudioIsPlayChanged(isPlay);
		};

		player->OnPlayingEnd = [m]() {
			m->CallOnAudioPlayingEnd();
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
		_audioController.CreateAudioPlayer(MainSoundKey);

		// 通知をリンク
		LinkEvent(this);
	}

	SoundEffectToolVM::~SoundEffectToolVM() {
		this->!SoundEffectToolVM();
	}

	SoundEffectToolVM::!SoundEffectToolVM() {
		delete _manager;
	}

	void SoundEffectToolVM::CreateDxView(IntPtr windowHandle, String^ windowName, Size size) {
		auto hwnd = reinterpret_cast<HWND>(windowHandle.ToPointer());
		_manager->CreateDxView(hwnd, ToStdString(windowName), PointInt((int)size.Width, (int)size.Height));
	}

	const IntPtr SoundEffectToolVM::GetBackBuffer(String^ windowName) {
		auto&& renderer = _manager->GetRenderer(ToStdString(windowName));
		if (!renderer) return IntPtr(nullptr);
		return IntPtr(const_cast<void*>(renderer->GetBackBuffer()));
	}

	float SoundEffectToolVM::GetDefaultPixelsPerSec() {
		return _manager->DefaultPixelsPerSec;
	}

	bool SoundEffectToolVM::ChangeDrawSize(String^ windowName, Size size) {
		auto&& renderer = _manager->GetRenderer(ToStdString(windowName));
		if (!renderer) return false;

		renderer->ChangeDrawSize(PointInt((int)size.Width, (int)size.Height));
		return true;
	}

	Size SoundEffectToolVM::GetDrawSize(String^ windowName) {
		auto&& renderer = _manager->GetRenderer(ToStdString(windowName));
		auto size = renderer->GetDrawSize();
		return Size(size.X, size.Y);
	}

	bool SoundEffectToolVM::Draw(String^ windowName, Point offset, float playRatio) {
		auto&& renderer = _manager->GetRenderer(ToStdString(windowName));
		if (!renderer) return false;

		renderer->Draw(PointInt((int)offset.X, (int)offset.Y), playRatio);
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
		return _manager->SetSoundData(MainSoundKey, ToStdString(key));
	}

	bool SoundEffectToolVM::SetWaveData(String^ windowName, String^ key) {
		return _manager->SetWaveData(ToStdString(windowName), ToStdString(key));
	}

	bool SoundEffectToolVM::CalcWaveRenderingScale(String^ windowName, float pixelsPerSec) {
		return _manager->CalcWaveRenderingScale(ToStdString(windowName), pixelsPerSec);
	}

	bool SoundEffectToolVM::IsPlay() {
		return _audioController.GetAudioPlayer(MainSoundKey)->IsPlay();
	}

	float SoundEffectToolVM::GetVolume() {
		return _audioController.GetAudioPlayer(MainSoundKey)->GetVolume();
	}

	void SoundEffectToolVM::SetVolume(float volume) {
		_audioController.GetAudioPlayer(MainSoundKey)->SetVolume(volume);
	}

	float SoundEffectToolVM::GetMainPlayerPosition() {
		return _audioController.GetAudioPlayer(MainSoundKey)->GetPlayPosition();
	}

	float SoundEffectToolVM::GetSoundFileLength() {
		return _audioController.GetAudioPlayer(MainSoundKey)->GetFileLength();
	}

	bool SoundEffectToolVM::PlayMainSound() {
		return _audioController.GetAudioPlayer(MainSoundKey)->PlayStart();
	}

	bool SoundEffectToolVM::PlayMainSoundAtPosition(float position) {
		return _audioController.GetAudioPlayer(MainSoundKey)->PlayStart(position);
	}

	bool SoundEffectToolVM::StopMainSound() {
		return _audioController.GetAudioPlayer(MainSoundKey)->Stop();
	}

	bool SoundEffectToolVM::PauseMainSound() {
		return _audioController.GetAudioPlayer(MainSoundKey)->Pause();
	}
}
