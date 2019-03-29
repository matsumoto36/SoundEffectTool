#pragma once

#include <math.h>
#include <memory>

#include "AudioDefine.h"
#include "AudioData.h"

namespace AudioLibrary {

	// �����f�[�^���Đ�����N���X
	class AUDIOLIBRARY_API AudioPlayer {

	private:
			
		bool _isPlay;		// �Đ����Ă��邩
		float _volume;		// ���݂̉���

		struct Impl;
		unique_ptr<Impl> _impl;

	public:
		AudioPlayer(IXAudio2& xAudio2, float volume = 1.0f);
		~AudioPlayer();

		// �R�s�[�͋֎~���邪�A���[�u�͋�����
		AudioPlayer(const AudioPlayer&) = delete;
		AudioPlayer(AudioPlayer&&) noexcept;
		AudioPlayer& operator=(const AudioPlayer&) = delete;

		// ���݉������Đ����Ă��邩
		bool IsPlay() const {
			return _isPlay;
		}

		float GetVolume() const {
			return _volume;
		}

		void SetVolume(float volume);

		// �Đ��V�X�e���̏����X�V����
		void Update();

		// �v���C���[�ɉ����f�[�^����͂���
		HRESULT SetAudioData(const shared_ptr<AudioData>& audioData);

		// �v���C���[���特���f�[�^���폜����
		void UnSetAudioData();

		// �����Đ�����
		HRESULT Play() const;

		// �����~�߂�
		HRESULT Stop() const;

		// �����|�[�Y����
		HRESULT Pause() const;
	};

	// �v���C���[�̃f���[�^�[
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

