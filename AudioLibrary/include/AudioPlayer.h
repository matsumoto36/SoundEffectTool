#pragma once

#include <math.h>
#include <memory>
#include <functional>

#include "AudioDefine.h"
#include "AudioPlayerStatus.h"
#include "AudioData.h"

namespace AudioLibrary {

	// �����f�[�^���Đ�����N���X
	class AUDIOLIBRARY_API AudioPlayer {

	public:
		function<void(bool)> OnIsPlayChanged;			// �Đ���Ԃ��ω������Ƃ��ɌĂ΂��
		function<void(void)> OnPlayingEnd;				// �Ō�܂ōĐ����ꂽ�Ƃ��ɌĂ΂��
		function<void(float)> OnVolumeChanged;			// ���ʂ��ω������Ƃ��ɌĂ΂��

	private:
		AudioPlayerStatus _status;		// �Đ����
		float _volume;					// ���݂̉���
		uint64_t _position;				// �Đ��ʒu(�T���v����)
		uint64_t _seekData;				// �Đ��ʒu(�o�C�g��)

		float _targetTime;				// �t�F�[�h���s������
		float _fading = 0;				// �t�F�[�h�̌o�ߎ���
		float _fadeStartVolume;			// �t�F�[�h�J�n���̉���
		float _targetVolume;			// �t�F�[�h���ɖڎw������
		function<void()> _fadeCallback;	// �t�F�[�h�����������Ƃ��Ɏ��s�����

		class VoiceCallback;			// XAudio2�̃R�[���o�b�N

		struct Impl;
		unique_ptr<Impl> _impl;

	public:
		AudioPlayer(IXAudio2& xAudio2, float volume = 1.0f);
		~AudioPlayer();

		// �R�s�[�͋֎~���邪�A���[�u�͋�����
		AudioPlayer(const AudioPlayer&) = delete;
		AudioPlayer& operator=(const AudioPlayer&) = delete;

		// ���݉������Đ����Ă��邩
		bool IsPlay() const {
			return _status == AudioPlayerStatus::Play;
		}

		float GetVolume() const {
			return _volume;
		}

		void SetVolume(float volume);

		// �Đ��ʒu���擾
		UINT64 GetPosition() const {
			return _position;
		}

		const shared_ptr<AudioData>& GetAudioData() const;

		// �v���C���[�ɉ����f�[�^����͂���
		HRESULT SetAudioData(const shared_ptr<AudioData>& audioData);

		// �v���C���[���特���f�[�^���폜����
		void UnSetAudioData();

		// �Đ��V�X�e���̏����X�V����
		void Update(float deltaTime);

		// �����Đ�����
		HRESULT Play(uint32_t samples);

		// �����~�߂�
		HRESULT Stop();

		// �����|�[�Y����
		HRESULT Pause();

	private:

		// �Đ��̏�Ԃ�ݒ肷��
		void SetPlayerStatus(AudioPlayerStatus status);

		// �f�[�^�������Ă��邩���ׂ�
		HRESULT CheckValidData() const;

		void AddBuffer(uint32_t requredBytes);

		// �����Ŏg�p���邽�߂̃t�F�[�h
		void SetFade(float targetVolume, float targetTime, function<void()> callback = nullptr);

		// �t�F�[�h�X�V
		void UpdateFade(float deltaTime);

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

