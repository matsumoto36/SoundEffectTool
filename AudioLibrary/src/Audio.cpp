// AudioLibrary.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//


#include <stdio.h>

#include "stdafx.h"
#include "../include/Audio.h"

namespace AudioLibrary {

	class Audio::Impl {
	public:
		unique_ptr<IXAudio2> _xAudio2;						// XAudio2のシステム
		unique_ptr<IXAudio2MasteringVoice> _masteringVoice;	// マスターボイス
	};

	// インスタンスだけ先に作っておく
	Audio& Audio::_instance = GetInstance();
	bool Audio::_isInitialized = false;

	Audio::Audio() :
		_impl(new Audio::Impl()) {}

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
		{
			IXAudio2* xAudio2 = nullptr;
			if (FAILED(hr = XAudio2Create(&xAudio2))) {
				wprintf(L"Failed to init XAudio2 engine: %#X\n", hr);
				CoUninitialize();
				return;
			}
			_impl->_xAudio2 = unique_ptr<IXAudio2>(xAudio2);

		}

		#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
		XAUDIO2_DEBUG_CONFIGURATION debug = { 0 };
		debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
		debug.BreakMask = XAUDIO2_LOG_ERRORS;
		_impl->_xAudio2->SetDebugConfiguration(&debug, 0);
		#endif

		// マスターボイスの作成
		{
			IXAudio2MasteringVoice* masteringVoice = nullptr;
			if (FAILED(hr = _impl->_xAudio2->CreateMasteringVoice(&masteringVoice))) {
				wprintf(L"Failed creating mastering voice: %#X\n", hr);
				_impl->_xAudio2->Release();
				CoUninitialize();
				return;
			}
			_impl->_masteringVoice = unique_ptr<IXAudio2MasteringVoice>(masteringVoice);
		}

		_isInitialized = true;
	}

	void Audio::Finalize() {
		if (!_isInitialized) return;

		// マスターボイスの破棄
		_impl->_masteringVoice->DestroyVoice();

		// XAudio2の破棄
		_impl->_xAudio2->Release();

		// COMの破棄
		CoUninitialize();

		_isInitialized = false;
	}

	unique_ptr<AudioPlayer, AudioPlayerDeleter> Audio::CreateAudioPlayer() {
		if (!_isInitialized) return nullptr;
		auto player = unique_ptr<AudioPlayer, AudioPlayerDeleter>(new AudioPlayer(*_impl->_xAudio2));
		return player;
	}

	shared_ptr<AudioData> Audio::LoadAudioData(const wstring& filePath) {
		if (!_isInitialized) return nullptr;

		shared_ptr<AudioData> data;
		auto hr = S_OK;
		if (FAILED(hr = AudioLoader::LoadWaveFile(*_impl->_xAudio2, filePath, data))) {
			return nullptr;
		}
		
		return move(data);
	}
}
