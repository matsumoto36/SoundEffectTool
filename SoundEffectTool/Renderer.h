#pragma once

#define SOUNDEFFECTTOOL_API __declspec(dllexport)

#include <Windows.h>

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API Renderer {

		bool isInitialized = false;
		int _viewWidth, _viewHeight;		// 出力される画面のサイズ

	public:
		Renderer();
		~Renderer();

		void Initialize(HWND, int, int);
		void Finalize();

		// バックバッファを取得する
		const void* GetBackBuffer() const;

		// 描画のサイズを変更する
		void ChangeDrawSize(int width, int height) const;

	};
}
