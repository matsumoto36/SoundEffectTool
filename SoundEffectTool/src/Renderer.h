#pragma once

#include "SoundEffectToolDefine.h"
#include <Windows.h>

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API Renderer {

		bool _isInitialized = false;
		int _maxWidth, _maxHeight;		// �o�͂�����ʂ̍ő�T�C�Y
		int _width, _height;

	public:
		Renderer();
		~Renderer();

		void Initialize(HWND, int, int);
		void Finalize();

		// �o�b�N�o�b�t�@���擾����
		const void* GetBackBuffer() const;

		// �`��̃T�C�Y��ύX����
		void ChangeDrawSize(int width, int height);

		// �`����X�V����
		void Draw() const;

	};
}
