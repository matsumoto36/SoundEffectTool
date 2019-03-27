#pragma once

#include <memory>

#include "AudioDefine.h"
#include "AudioData.h"

namespace AudioLibrary {

	typedef int AudioPlayerHandler;

	// �����f�[�^���Đ�����N���X
	class AUDIOLIBRARY_API AudioPlayer {
	public:
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

