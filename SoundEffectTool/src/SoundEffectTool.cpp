// SoundEffectToolModel.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"

#include "SoundEffectTool.h"

namespace SoundEffectTool {

	SoundEffectToolManager::SoundEffectToolManager() {
		
		// サウンドシステムの初期化
		//auto&& audio = Audio::GetInstance();
		//audio.Initialize();

		// プレーヤーを作る
		//auto player = audio.CreateAudioPlayer();
		//_audioPlayer = move(player);
	}

	SoundEffectToolManager::~SoundEffectToolManager() {
	
		// プレイヤーの破棄
		//_audioPlayer.reset();

		// サウンドシステムの破棄
		//Audio::GetInstance().Finalize();

	}

	void SoundEffectToolManager::CreateDxView(HWND windowHandle, string& rendererName, int width, int height) {
		auto renderer = make_unique<Renderer>();
		renderer->Initialize(windowHandle, width, height);
		_rendererList.emplace(rendererName, move(renderer));
	}

	const Renderer& SoundEffectToolManager::GetRenderer(string& rendererName) {
		return *_rendererList[rendererName];
	}

	void SoundEffectToolManager::PlaySoundFromFile(const wstring& filePath) const {
		// ファイル読み込み
		auto audioData = move(Audio::GetInstance().LoadAudioData(filePath));

		if (!audioData) return;
		
		// データをセット
		//_audioPlayer->SetAudioData(audioData);
		// 再生
		//_audioPlayer->Play();
	}

}