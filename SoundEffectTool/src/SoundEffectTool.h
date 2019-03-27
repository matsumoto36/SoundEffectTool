#pragma once

// unique_ptr�g�p�ɂ�
#pragma warning(disable:4251)

#include <vector>
#include <memory>
#include <map>

#include "SoundEffectToolDefine.h"
#include "Renderer.h"

using namespace std;

namespace SoundEffectTool {

	using namespace AudioLibrary;

	class SOUNDEFFECTTOOL_API SoundEffectToolManager final sealed {

		map<string, unique_ptr<Renderer>> _rendererList;
		AudioPlayer& _audioPlayer;

	public:

		// �R�s�[�͋֎~���邪�A���[�u�͋�����
		SoundEffectToolManager(const SoundEffectToolManager&) = delete;
		SoundEffectToolManager& operator=(const SoundEffectToolManager&) = delete;

		SoundEffectToolManager();
		~SoundEffectToolManager();
		
		// �E�B���h�E�̐���
		void CreateDxView(HWND windowHandle, string& rendererName, int width, int height);

		const Renderer& GetRenderer(string& rendererName);

		// �������Đ�
		void PlaySoundFromFile(const wstring& filePath) const;

	private:

	};
}
