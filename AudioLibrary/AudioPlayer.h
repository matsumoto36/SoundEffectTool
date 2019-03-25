#pragma once

#include <memory>
#include "AudioData.h"

namespace {

	// �\�[�X�{�C�X�̃f���[�^
	struct SourceVoiceDeleter {
		void operator()(IXAudio2SourceVoice* voice) const {
			voice->DestroyVoice();
		}
	};

}

namespace AudioLibrary {

	// �����f�[�^���Đ�����N���X
	class AUDIOLIBRARY_API AudioPlayer {

	public:

	private:

		IXAudio2& _xAudio2;
		
		shared_ptr<AudioData> _audioData;									// �Đ����鉹���f�[�^
		unique_ptr<IXAudio2SourceVoice, SourceVoiceDeleter> _sourceVoice;	// �\�[�X�{�C�X

	public:
		AudioPlayer(const unique_ptr<IXAudio2>& xAudio2);
		~AudioPlayer();

		// �R�s�[�͋֎~���邪�A���[�u�͋�����
		AudioPlayer(const AudioPlayer&) = delete;
		AudioPlayer& operator=(const AudioPlayer&) = delete;

		// �v���C���[�ɉ����f�[�^����͂���
		HRESULT SetAudioData(const shared_ptr<AudioData>& audioData);

		// �v���C���[���特���f�[�^���폜����
		void UnSetAudioData();

		// ���݉������Đ����Ă��邩
		bool IsPlay();

		// �����Đ�����
		HRESULT Play();

		// �����~�߂�
		HRESULT Stop();

		// �����|�[�Y����
		HRESULT Pause();
	};
}

