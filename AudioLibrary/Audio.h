#pragma once

// unique_ptr使用につき
#pragma warning(disable:4251)

#include <memory>

namespace {

	// XAudio2のデリータ
	struct XAudio2Deleter {
		void operator()(IXAudio2* xAudio2) const {
			xAudio2->Release();
		}
	};

	// マスターボイスのデリータ
	struct MastringVoiceDeleter {
		void operator()(IXAudio2MasteringVoice* voice) const {
			voice->DestroyVoice();
		}
	};

}

namespace AudioLibrary {

	using namespace std;

	// サウンドシステムを管理するクラス
	class AUDIOLIBRARY_API Audio final sealed {

		static bool _isInitialized;
		static unique_ptr<IXAudio2, XAudio2Deleter> _xAudio2;
		static unique_ptr<IXAudio2MasteringVoice, MastringVoiceDeleter> _masteringVoice;

	public:
		static void Initialize();
		static void Finalize();

		static void PlayWave();
	};
}