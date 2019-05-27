#pragma once

// unique_ptr使用につき
#pragma warning(disable:4251)

#include <vector>
#include <memory>

#include "PointInt.h"
#include "SoundEffectToolDefine.h"

using namespace AudioLibrary;
using namespace std;

namespace SoundEffectTool {

	// 主に波形を表示するクラス
	class SOUNDEFFECTTOOL_API Renderer {

		bool _isInitialized = false;
		PointInt _maxSize;							// 出力される画面の最大サイズ
		PointInt _drawSize;							// 波形の描画サイズ

		int _waveRenderHandler = -1;				// 描画された波形のハンドラ
		int _waveMarginY = 30;						// 波形との間の距離
		int _drawMarginY = 10;						// 上下の間の距離
		shared_ptr<AudioData> _audioData = nullptr;	// セットされた波形データ

	public:

		// コピーは禁止するが、ムーブは許可する
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		Renderer();
		~Renderer();

		void Initialize(HWND hwnd, PointInt size);
		void Finalize();

		// バックバッファを返す
		const void* GetBackBuffer() const;

		// 波形の情報を返す
		const shared_ptr<AudioData> GetAudioData() {
			return _audioData;
		}

		// 波形の情報を設定する
		void SetAudioData(const shared_ptr<AudioData> audioData) {
			_audioData = audioData;
		}

		// ピクセルの位置と高さから、波形の描画を事前計算をする(sampleStart, sampleLengthはチャンネル倍の値になる)
		void CalcRenderingData(PointInt pixelSize, uint32_t sampleStart, uint32_t sampleLength, uint32_t samplesPerSec = 1000);

		// 描画のサイズを変更する
		void ChangeDrawSize(PointInt size);

		// 描画のサイズを返す
		PointInt GetDrawSize() const {
			return _drawSize;
		}

		// 描画を更新する
		void Draw(PointInt waveOffset, float playRatio) const;

	private:

		// 波形を描画する
		void DrawWave(vector<vector<PointInt>> wavePoints, PointInt pixelSize) const;
	};
}
