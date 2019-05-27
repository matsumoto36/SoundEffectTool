// SoundEffectToolModel.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"

#include "../include/SoundEffectTool.h"

namespace SoundEffectTool {

	SoundEffectToolManager::SoundEffectToolManager() :
		_audioController(make_unique<AudioController>()) { }

	SoundEffectToolManager::~SoundEffectToolManager() { }

	void SoundEffectToolManager::CreateDxView(HWND windowHandle, string& rendererName, PointInt size) {
		auto renderer = make_unique<Renderer>();
		renderer->Initialize(windowHandle, size);
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

		return player->SetSoundData(audioData);;
	}

	bool SoundEffectToolManager::SetWaveData(string& rendererName, string& key) {
		auto&& renderer = GetRenderer(rendererName);
		if (!renderer) return false;

		auto audioData = _audioController->GetAudioData(key);
		if (!audioData) return false;

		// 情報を取得して投げる
		renderer->SetAudioData(audioData);

		// デフォルトの設定で描画データ準備
		return CalcWaveRenderingScale(rendererName, DefaultPixelsPerSec);;
	}

	bool SoundEffectToolManager::CalcWaveRenderingScale(string& rendererName, float pixelsPerSec) {
		auto&& renderer = GetRenderer(rendererName);
		if (!renderer) return false;

		auto audioData = renderer->GetAudioData();
		if (!audioData) return false;

		// pixelsPerSecから波形を表示するサイズを計算
		auto samplingRate = audioData->GetFormat().nSamplesPerSec;
		auto length = audioData->GetSampleLength() / audioData->GetChannelCount();
		auto sec = (float)length / samplingRate;
		auto width = int(sec * pixelsPerSec);
		renderer->CalcRenderingData(PointInt(width, DefaultWaveHeight), 0, length);
		return true;
	}

}