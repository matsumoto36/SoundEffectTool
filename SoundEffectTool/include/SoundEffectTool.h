#pragma once

// unique_ptr�g�p�ɂ�
#pragma warning(disable:4251)

#include <string>

#include "SoundEffectToolDefine.h"
#include "Renderer.h"

using namespace std;

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API SoundEffectToolManager final sealed {

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
