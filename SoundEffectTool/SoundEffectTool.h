#pragma once

#define SOUNDEFFECTTOOL_API __declspec(dllexport)

#include <vector>
#include <memory>

#include "Renderer.h"
#include "Extensions.h"

namespace AudioLibrary {
	class Audio;
}

using namespace std;
using namespace AudioLibrary;

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API SoundEffectToolManager final sealed {

		static SoundEffectToolManager* _instance;

		vector<unique_ptr<Renderer>> _rendererList = Extensions::MakeUniqueVector<Renderer>();

	public:

		static bool Initialize();
		static void Final();

		// �E�B���h�E�̐���
		// �߂�l�̓o�b�N�o�b�t�@
		static const void* CreateDxView(HWND windowHandle);
		
	private:
		
		SoundEffectToolManager();
		~SoundEffectToolManager();

	};
}
