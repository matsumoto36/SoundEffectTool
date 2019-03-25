#pragma once

// unique_ptr�g�p�ɂ�
#pragma warning(disable:4251)

#include <memory>

namespace {

	// XAudio2�̃f���[�^
	struct XAudio2Deleter {
		void operator()(IXAudio2* xAudio2) const {
			xAudio2->Release();
		}
	};

	// �}�X�^�[�{�C�X�̃f���[�^
	struct MastringVoiceDeleter {
		void operator()(IXAudio2MasteringVoice* voice) const {
			voice->DestroyVoice();
		}
	};

}

namespace AudioLibrary {

	using namespace std;

	// �T�E���h�V�X�e�����Ǘ�����N���X
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