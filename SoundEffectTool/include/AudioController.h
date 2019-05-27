#pragma once

#include <map>
#include <memory>
#include <functional>

#include "AudioPlayer.h"
#include "SoundEffectToolDefine.h"

using namespace std;
using namespace AudioLibrary;

namespace SoundEffectTool {

	// ���C�u�����̃T�E���h�V�X�e���𐧌䂷��N���X
	class SOUNDEFFECTTOOL_API AudioController {

	private:

		map<string, shared_ptr<AudioPlayer>> _audioPlayerList;	// ���������v���C���[�̃e�[�u��
		map<string, shared_ptr<AudioData>> _audioDataList;		// �ǂݍ��񂾉����f�[�^�̃e�[�u��

	public:
		// �R�s�[�͋֎~���邪�A���[�u�͋�����
		AudioController(const AudioController&) = delete;
		AudioController& operator=(const AudioController&) = delete;

		AudioController();
		~AudioController();

		// �v���C���[���쐬����(key���A�N�Z�X�L�[�Ƃ��ēo�^)
		bool CreateAudioPlayer(const string& key);

		// �v���C���[���擾����
		shared_ptr<AudioPlayer> GetAudioPlayer(const string& key);

		// �����f�[�^���擾����
		shared_ptr<AudioData> GetAudioData(const string& key);

		// �����X�V����
		void Update(float deltaTime) const;

		// ���������[�h����(key���A�N�Z�X�L�[�Ƃ��ēo�^)
		bool LoadSound(const wstring& filePath, const string& key);

		// �������������
		bool UnLoadSound(const string& key);

	private:

		// �v���C���[�����݂��Ă��邩
		bool IsValidAudioPlayer(const string& key) const;

		// �f�[�^�����݂��Ă��邩
		bool IsValidAudioData(const string& key) const;

	};

}