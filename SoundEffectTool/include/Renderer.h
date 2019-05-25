#pragma once

// unique_ptr使用につき
#pragma warning(disable:4251)

#include <vector>
#include <memory>

#include "SoundEffectToolDefine.h"

using namespace AudioLibrary;
using namespace std;

namespace SoundEffectTool {

	// 点を表す
	struct SOUNDEFFECTTOOL_API PointInt {
	public:
		int X, Y;
		PointInt() : X(0), Y(0) {}
		PointInt(int x, int y) : X(x), Y(y) {}
		PointInt operator+(PointInt p) {
			return PointInt(X + p.X, Y + p.Y);
		}
		PointInt operator-(PointInt p) {
			return PointInt(X - p.X, Y - p.Y);
		}
	};

	// 線を表す
	struct SOUNDEFFECTTOOL_API LineInt {
	public:
		PointInt From, To;
		LineInt() : From(PointInt()), To(PointInt()) {}
		LineInt(int x1, int y1, int x2, int y2) : From(PointInt(x1, y1)), To(PointInt(x2, y2)) {}
		LineInt(PointInt from, PointInt to) : From(from), To(to) {}
	};

	// 主に波形を表示するクラス
	class SOUNDEFFECTTOOL_API Renderer {

		bool _isInitialized = false;
		int _maxWidth, _maxHeight;		// 出力される画面の最大サイズ
		int _width, _height;

		uint32_t _waveLength = 0;
		uint32_t _waveWidth = 1024;
		uint32_t _waveHeight = 100;
		shared_ptr<AudioData> _audioData = nullptr;
		vector<vector<PointInt>> _waveDrawingPoints;

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

		// ピクセルの位置と高さから、波形の描画を事前計算をする(sampleStart, sampleLengthはチャンネル倍の値になる)
		void CalcRenderingData(uint32_t pixelWidth, uint32_t pixelHeight, uint32_t sampleStart, uint32_t sampleLength);

		// 描画のサイズを変更する
		void ChangeDrawSize(int width, int height);

		// 描画を更新する
		void DrawWave() const;

	};
}
