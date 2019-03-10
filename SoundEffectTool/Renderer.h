#pragma once

#define SOUNDEFFECTTOOL_API __declspec(dllexport)

#include <Windows.h>

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API Renderer {

		bool isInitialized = false;
		int _maxWidth, _maxHeight;		// 出力される画面の最大サイズ
		int _width, _height;

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
