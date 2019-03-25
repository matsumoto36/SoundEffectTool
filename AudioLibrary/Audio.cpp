// AudioLibrary.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//


#include <stdio.h>

#include "stdafx.h"
#include "Audio.h"

namespace AudioLibrary {

	bool Audio::_isInitialized = false;
	unique_ptr<IXAudio2, XAudio2Deleter> Audio::_xAudio2;
	unique_ptr<IXAudio2MasteringVoice, MastringVoiceDeleter> Audio::_masteringVoice;

	void Audio::Initialize() {

		if (_isInitialized) {
			wprintf(L"XAudio2 is already initialized. \n");
			return;
		}

		// COMの初期化
		CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

		UINT32 flags = 0;
		#if (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
		flags |= XAUDIO2_DEBUG_ENGINE;
		#endif

		auto hr = S_OK;

		// XAudio2の初期化
		{
			IXAudio2* pXAudio2 = nullptr;
			if (FAILED(hr = XAudio2Create(&pXAudio2, flags))) {
				wprintf(L"Failed to init XAudio2 engine: %#X\n", hr);
				CoUninitialize();
				return;
			}

			_xAudio2 = unique_ptr<IXAudio2, XAudio2Deleter>(pXAudio2);
		}


		#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
		XAUDIO2_DEBUG_CONFIGURATION debug = { 0 };
		debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
		debug.BreakMask = XAUDIO2_LOG_ERRORS;
		_xAudio2->SetDebugConfiguration(&debug, 0);
		#endif

		// マスターボイスの作成
		{
			IXAudio2MasteringVoice* masteringVoice;
			if (FAILED(hr = _xAudio2->CreateMasteringVoice(&masteringVoice))) {
				wprintf(L"Failed creating mastering voice: %#X\n", hr);
				_xAudio2.reset();
				CoUninitialize();
				return;
			}

			_masteringVoice = unique_ptr<IXAudio2MasteringVoice, MastringVoiceDeleter>(masteringVoice);
		}


		_isInitialized = true;
	}

	void Audio::Finalize() {
		if (!_isInitialized) return;

		// マスターボイスの破棄
		_masteringVoice.reset();

		// XAudio2の破棄
		_xAudio2.reset();

		// COMの破棄
		CoUninitialize();
	}

	void Audio::PlayWave() {
		if (!_isInitialized) return;


	}
}
