// SoundEffectToolModel.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"

#include "../include/SoundEffectTool.h"
#include "AudioPlayer.h"
#include "AudioData.h"

namespace SoundEffectTool {

	struct SoundEffectToolManager::Impl {
		map<string, unique_ptr<AudioPlayer, AudioPlayerDeleter>> _audioPlayerList;
		map<string, shared_ptr<AudioData>> _audioDataList;
	};


	SoundEffectToolManager::SoundEffectToolManager() :
		_impl(make_unique<Impl>()) {
		
		// サウンドシステムの初期化
		auto&& audio = Audio::GetInstance();
		audio.Initialize();

		// プレーヤーを作る
		auto player = audio.CreateAudioPlayer();
		_impl->_audioDataList.emplace("Main", move(player));
	}

	SoundEffectToolManager::~SoundEffectToolManager() {
	
		// プレイヤーの破棄
		for (auto&& player : _impl->_audioPlayerList) {
			player.second.reset();
		}

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

	bool SoundEffectToolManager::LoadSound(const wstring& filePath, const string& name) const {
		
		// 同じキーで登録できないようにする
		if (_impl->_audioDataList.count(name) > 0) {
			printf("already loaded sound data. key=%s", name);
		};
		
		// ファイル読み込み
		auto audioData = move(Audio::GetInstance().LoadAudioData(filePath));

		if (!audioData) {
			wprintf(L"failed load sound data. path=%s", filePath);
		}
		
		// 追加
		_impl->_audioDataList.emplace(name, move(audioData));
	}

	bool SoundEffectToolManager::PlaySound(const string& name) const {

		if (_impl->_audioDataList.count(name) <= 0) {
			printf("Sound data is not found. key=%s", name);
			return false;
		}

		auto data = _impl->_audioDataList[name];
		auto&& target = _impl->_audioPlayerList["Main"];
		// データをセット
		target->SetAudioData(data);
		// 再生
		target->Play();
	}

}