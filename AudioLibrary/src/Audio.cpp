// AudioLibrary.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//


#include <stdio.h>

#include "stdafx.h"
#include "Audio.h"

namespace AudioLibrary {

	// インスタンスだけ先に作っておく
	Audio& Audio::_instance = GetInstance();
	bool Audio::_isInitialized = false;

	Audio& Audio::GetInstance() {
		static Audio audio;
		return audio;
	}

	void Audio::Initialize() {
		_RPT0(_CRT_WARN, "Audio::Initialize\n");
		wprintf(L"Audio::Initialize\n");

		if (_isInitialized) {
			wprintf(L"XAudio2 is already initialized. \n");
			return;
		}

		return;

		// COMの初期化
		CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

		auto hr = S_OK;

		// XAudio2の初期化
		_xAudio2 = nullptr;
		if (FAILED(hr = XAudio2Create(&_xAudio2))) {
			wprintf(L"Failed to init XAudio2 engine: %#X\n", hr);
			CoUninitialize();
			return;
		}

		#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
		XAUDIO2_DEBUG_CONFIGURATION debug = { 0 };
		debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
		debug.BreakMask = XAUDIO2_LOG_ERRORS;
		_xAudio2->SetDebugConfiguration(&debug, 0);
		#endif

		// マスターボイスの作成
		_masteringVoice = nullptr;
		if (FAILED(hr = _xAudio2->CreateMasteringVoice(&_masteringVoice))) {
			wprintf(L"Failed creating mastering voice: %#X\n", hr);
			_xAudio2->Release();
			CoUninitialize();
			return;
		}

		_isInitialized = true;
	}

	void Audio::Finalize() {
		if (!_isInitialized) return;

		// マスターボイスの破棄
		_masteringVoice->DestroyVoice();

		// XAudio2の破棄
		_xAudio2->Release();

		// COMの破棄
		CoUninitialize();

		_isInitialized = false;
	}

	AudioPlayerHandler Audio::CreateAudioPlayer() {
		if (!_isInitialized) return nullptr;
		auto player = unique_ptr<AudioPlayer, AudioPlayerDeleter>(new AudioPlayer(*_xAudio2));
		return player;
	}

	shared_ptr<AudioData> Audio::LoadAudioData(const wstring& filePath) {
		if (!_isInitialized) return nullptr;

		shared_ptr<AudioData> data;
		auto hr = S_OK;
		if (FAILED(hr = AudioLoader::LoadWaveFile(*_xAudio2, filePath, data))) {
			return nullptr;
		}
		
		return move(data);
	}
}
