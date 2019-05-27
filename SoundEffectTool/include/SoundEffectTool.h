#pragma once

// unique_ptr�g�p�ɂ�
#pragma warning(disable:4251)

#include <vector>
#include <memory>
#include <map>

#include "Renderer.h"
#include "AudioController.h"
#include "SoundEffectToolDefine.h"

using namespace std;

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API SoundEffectToolManager final sealed {
		
		map<string, unique_ptr<Renderer>> _rendererList;	// �`��E�B���h�E�̃e�[�u��
		unique_ptr<AudioController> _audioController;		// �T�E���h�R���g���[���[

	public:

		const int DefaultWaveHeight = 100;					// �g�`�̕`��̍���
		const float DefaultPixelsPerSec = 64;				// 1�b�Ԃ̃f�[�^�����s�N�Z���ŕ\����

		// �R�s�[�͋֎~���邪�A���[�u�͋�����
		SoundEffectToolManager(const SoundEffectToolManager&) = delete;
		SoundEffectToolManager& operator=(const SoundEffectToolManager&) = delete;

		SoundEffectToolManager();
		~SoundEffectToolManager();
		
		// �E�B���h�E�̐���
		void CreateDxView(HWND windowHandle, string& rendererName, PointInt size);

		// �`���Ԃ�
		const unique_ptr<Renderer>& GetRenderer(string& rendererName);

		// �T�E���h�R���g���[���[��Ԃ�
		AudioController& GetAudioController() const;

		// �����f�[�^���v���C���[�ɃZ�b�g
		bool SetSoundData(string& playerKey, string& audioDataKey);

		// �g�`�f�[�^��`��ɃZ�b�g
		bool SetWaveData(string& rendererName, string& key);

		// �g�`�̕\�����X�P�[�����O����
		bool CalcWaveRenderingScale(string& rendererName, float pixelsPerSec);

	private:

	};
}
