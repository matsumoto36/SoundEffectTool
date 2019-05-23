#pragma once

// unique_ptr使用につき
#pragma warning(disable:4251)

#include <vector>
#include <memory>

#include "SoundEffectToolDefine.h"

using namespace AudioLibrary;
using namespace std;

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API Renderer {

		bool _isInitialized = false;
		int _maxWidth, _maxHeight;		// 出力される画面の最大サイズ
		int _width, _height;

		UINT32 _waveLength = 0;
		int _waveWidth = 1024;
		int _waveHeight = 100;
		uint8_t _waveMax;
		shared_ptr<AudioData> _audioData = nullptr;
		vector<unique_ptr<uint8_t[]>> _waveDrawingData;

	public:

		// コピーは禁止するが、ムーブは許可する
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		Renderer();
		~Renderer();

		void Initialize(HWND, int, int);
		void Finalize();

		// バックバッファを取得する
		const void* GetBackBuffer() const;

		// 波形の情報を変換してセットする
		void SetAudioData(const shared_ptr<AudioData> audioData);

		// 波形の長さと位置を変更
		void SetRenderingData(int waveWidth, int waveHeight, UINT32 start, UINT32 end);

		// 描画のサイズを変更する
		void ChangeDrawSize(int width, int height);

		// 描画を更新する
		void DrawWave() const;

	};
}
