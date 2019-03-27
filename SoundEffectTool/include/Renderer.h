#pragma once

#include "SoundEffectToolDefine.h"

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API Renderer {
	public:
		Renderer();
		~Renderer();

		void Initialize(HWND, int, int);
		void Finalize();

		// バックバッファを取得する
		const void* GetBackBuffer() const;

		// 描画のサイズを変更する
		void ChangeDrawSize(int width, int height);

		// 描画を更新する
		void Draw() const;

	};
}
