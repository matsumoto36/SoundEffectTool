#pragma once

#include <Windows.h>

namespace SoundEffectTool {

	class Renderer {

		int _viewWidth, _viewHeight;		// 出力される画面のサイズ

	public:
		Renderer();
		~Renderer();

		void Initialize(HWND);
		void Finalize();

		const void* GetBackBuffer() const;
	};
}
