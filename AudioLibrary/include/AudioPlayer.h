#pragma once

#include <memory>

#include "AudioDefine.h"
#include "AudioData.h"

namespace AudioLibrary {

	// �����f�[�^���Đ�����N���X
	class AUDIOLIBRARY_API AudioPlayer {

	public:

	private:

		IXAudio2& _xAudio2;
		
		shared_ptr<AudioData> _audioData;	// �Đ����鉹���f�[�^
		IXAudio2SourceVoice* _sourceVoice;	// �\�[�X�{�C�X

	public:
		AudioPlayer(IXAudio2& xAudio2);
		~AudioPlayer();

		// �R�s�[�͋֎~���邪�A���[�u�͋�����
		AudioPlayer(const AudioPlayer&) = delete;
		AudioPlayer(AudioPlayer&&) noexcept = default;
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

	struct AudioPlayerDeleter {
		void operator()(AudioPlayer* player) const {
			// �Đ����Ȃ�~�߂�
			player->Stop();

			// ���\�[�X�̊J��
			player->UnSetAudioData();

			delete player;
		}
	};
}

