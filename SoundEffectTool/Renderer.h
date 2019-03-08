#pragma once

#define SOUNDEFFECTTOOL_API __declspec(dllexport)

#include <Windows.h>

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API Renderer {

		bool isInitialized = false;
		int _viewWidth, _viewHeight;		// �o�͂�����ʂ̃T�C�Y

	public:
		Renderer();
		~Renderer();

		void Initialize(HWND, int, int);
		void Finalize();

		// �o�b�N�o�b�t�@���擾����
		const void* GetBackBuffer() const;

		// �`��̃T�C�Y��ύX����
		void ChangeDrawSize(int width, int height) const;

	};
}
