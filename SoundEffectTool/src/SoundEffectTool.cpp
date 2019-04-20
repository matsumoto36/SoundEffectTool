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

	const unique_ptr<Renderer>& SoundEffectToolManager::GetRenderer(string& rendererName) {
		return _rendererList[rendererName];
	}

	AudioController& SoundEffectToolManager::GetAudioController() const {
		return *_audioController;
	}

	bool SoundEffectToolManager::SetSoundData(string& playerKey, string& audioDataKey) {
		auto player = _audioController->GetAudioPlayer(playerKey);
		if (!player) return false;

		auto audioData = _audioController->GetAudioData(audioDataKey);
		if (!audioData) return false;

		player->SetSoundData(audioData);

		return true;
	}

	bool SoundEffectToolManager::SetWaveData(string& rendererName, string& key) {
		auto&& renderer = GetRenderer(rendererName);
		if (!renderer) return false;

		auto audioData = _audioController->GetAudioData(key);
		if (!audioData) return false;

		// 情報を取得して投げる
		renderer->SetAndSplitWaveData(audioData->GetWave(), audioData->GetLength(), audioData->GetChannelCount());

		return true;
	}
}