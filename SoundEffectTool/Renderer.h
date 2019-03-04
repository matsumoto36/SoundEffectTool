#pragma once

#include <Windows.h>

namespace SoundEffectTool {

	class Renderer {

		int _viewWidth, _viewHeight;		// �o�͂�����ʂ̃T�C�Y

	public:
		Renderer();
		~Renderer();

		void Initialize(HWND);
		void Finalize();

		const void* GetBackBuffer() const;
	};
}
