#pragma once

#include "SoundEffectToolDefine.h"

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API Renderer {
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
