#pragma once

#include <memory>
#include <functional>

#include "SoundEffectToolDefine.h"

using namespace std;
using namespace AudioLibrary;

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API AudioController {

	public:
		function<void(bool)> OnIsPlayChanged;	// �Đ���Ԃ��ω������Ƃ��ɌĂ΂��
		function<void(float)> OnVolumeChanged;	// ���ʂ��ω������Ƃ��ɌĂ΂��

	private:
		struct Impl;
		unique_ptr<Impl> _impl;

	public:
		AudioController();
		~AudioController();

		// �Đ������ǂ������擾����
		bool IsPlay() const;

		// �����X�V����
		void Update() const;

		// ���������[�h����
		bool LoadSound(const wstring& filePath, const string& name) const;

		// �������������
		bool UnLoadSound(const string& name) const;

		// �������Đ��\��Ԃɂ���
		bool SetMainSound(const string& name) const;

		// ���ʂ��擾����
		float GetVolume() const;

		// ���ʂ�ݒ肷��
		void SetVolume(float volume) const;

		// �������Đ�����
		bool PlayMainSound() const;

		// �������~����
		bool StopMainSound() const;
		
		// �������ꎞ��~����
		bool PauseMainSound() const;

	};

}