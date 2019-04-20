#pragma once

// unique_ptr�g�p�ɂ�
#pragma warning(disable:4251)

#include <vector>
#include <memory>
#include <map>

#include "SoundEffectToolDefine.h"
#include "Renderer.h"
#include "AudioController.h"

using namespace std;

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API SoundEffectToolManager final sealed {

		map<string, unique_ptr<Renderer>> _rendererList;
		unique_ptr<AudioController> _audioController;

	public:

		// �R�s�[�͋֎~���邪�A���[�u�͋�����
		SoundEffectToolManager(const SoundEffectToolManager&) = delete;
		SoundEffectToolManager& operator=(const SoundEffectToolManager&) = delete;

		SoundEffectToolManager();
		~SoundEffectToolManager();
		
		// �E�B���h�E�̐���
		void CreateDxView(HWND windowHandle, string& rendererName, int width, int height);

		const unique_ptr<Renderer>& GetRenderer(string& rendererName);

		AudioController& GetAudioController() const;

		// �����f�[�^���v���C���[�ɃZ�b�g
		bool SetSoundData(string& playerKey, string& audioDataKey);

		// �g�`�f�[�^��`��ɃZ�b�g
		bool SetWaveData(string& rendererName, string& key);

	private:

	};
}
