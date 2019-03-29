// SoundEffectToolModel.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"

#include "../include/SoundEffectTool.h"
#include "AudioPlayer.h"
#include "AudioData.h"

namespace SoundEffectTool {

	struct SoundEffectToolManager::Impl {
		unique_ptr<AudioPlayer, AudioPlayerDeleter> _audioPlayer;
		shared_ptr<AudioData> _audioData;
	};


	SoundEffectToolManager::SoundEffectToolManager() :
		_impl(make_unique<Impl>()) {
		
		// サウンドシステムの初期化
		auto&& audio = Audio::GetInstance();
		audio.Initialize();

		// プレーヤーを作る
		auto player = audio.CreateAudioPlayer();
		_impl->_audioPlayer = move(player);
	}

	SoundEffectToolManager::~SoundEffectToolManager() {
	
		// プレイヤーの破棄
		_impl->_audioPlayer.reset();

		// サウンドシステムの破棄
		Audio::GetInstance().Finalize();
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
		_impl->_audioPlayer->SetAudioData(audioData);
		// 再生
		_impl->_audioPlayer->Play();
	}

}