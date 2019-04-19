#pragma once

#include "SoundEffectToolDefine.h"
#include <Windows.h>

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API Renderer {

		bool _isInitialized = false;
		int _maxWidth, _maxHeight;		// 出力される画面の最大サイズ
		int _width, _height;

		vector<unique_ptr<uint8_t[]>> _wavePerChannel;

	public:
		Renderer();
		~Renderer();

		void Initialize(HWND, int, int);
		void Finalize();

		// バックバッファを取得する
		const void* GetBackBuffer() const;

		// 波形の情報をセットする
		void SetWaveData(const unique_ptr<uint8_t[]>& waveData, UINT32 length, int channels);

		// 描画のサイズを変更する
		void ChangeDrawSize(int width, int height);

		// 描画を更新する
		void DrawWave() const;

	};
}
