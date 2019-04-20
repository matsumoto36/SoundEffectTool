#pragma once

#include <memory>

#include "SoundEffectToolDefine.h"

using namespace std;
using namespace AudioLibrary;

namespace SoundEffectTool {

	// ���C�u�����̃v���C���[�����b�v��������
	class SOUNDEFFECTTOOL_API AudioPlayer {

	public:
		function<void(bool)> OnIsPlayChanged;	// �Đ���Ԃ��ω������Ƃ��ɌĂ΂��
		function<void(float)> OnVolumeChanged;	// ���ʂ��ω������Ƃ��ɌĂ΂��

	private:

		float _position;	// �Đ��ʒu(s)
		float _length;		// �t�@�C���̍Đ�����(s)

		struct Impl;
		unique_ptr<Impl> _impl;

	public:

		// �R�s�[�͋֎~���邪�A���[�u�͋�����
		AudioPlayer(const AudioPlayer&) = delete;
		AudioPlayer& operator=(const AudioPlayer&) = delete;

		AudioPlayer();
		~AudioPlayer();

		// �Đ������ǂ������擾����
		bool IsPlay() const;

		// ���݂̍Đ��ʒu���擾����
		float GetPlayPosition() const {
			return _position;
		}

		// �t�@�C���̍Đ����Ԃ��擾����
		float GetFileLength() const {
			return _length;
		}

		// �������Đ��\��Ԃɂ���
		bool SetSoundData(shared_ptr<AudioData> audioData);

		// ���ʂ��擾����
		float GetVolume() const;

		// ���ʂ�ݒ肷��
		void SetVolume(float volume) const;

		// �v���C���[�̏�Ԃ��X�V����
		void Update(float deltaTime);

		// �������Đ�����
		bool PlayStart(float position = 0);

		// �������~����
		bool Stop();

		// �������ꎞ��~����
		bool Pause() const;
	};

}