#include "stdafx.h"
#include "../include/Renderer.h"
#include "DxLib.h"

using namespace DxLib;

namespace SoundEffectTool {

	Renderer::Renderer() = default;
	Renderer::~Renderer() = default;

	void Renderer::Initialize(HWND hwnd, int width, int height) {

		// Dxライブラリ初期化設定
		SetUseDirect3DVersion(DX_DIRECT3D_9EX);				// DirectX 9EXにする
		SetUserWindow(hwnd);								// 描画ウィンドウの設定
		ChangeWindowMode(TRUE);								// ウィンドウモードに設定
		SetWindowSizeChangeEnableFlag(TRUE);				// ウィンドウサイズを可変に

		GetDefaultState(&_maxWidth, &_maxHeight, NULL);		// デスクトップのウィンドウサイズを取得
		SetGraphMode(_maxWidth, _maxHeight, 32);			// グラフィックモードの設定(最大)

		SetAlwaysRunFlag(TRUE);								// 非アクティブ時も処理続行
		SetDrawScreen(DX_SCREEN_BACK);						// 描画先をバックバッファへ設定
		SetUseFPUPreserveFlag(TRUE);						// FPUの精度を落とさない
		SetWaitVSyncFlag(FALSE);							// VSync同期を無効
		SetOutApplicationLogValidFlag(FALSE);				// ログ出力停止
		SetDoubleStartValidFlag(TRUE);						// 多重起動を許可
		SetUseIMEFlag(TRUE);								// IMEを有効

		// Dxライブラリの初期化
		if (DxLib_Init() == -1) return;

		// ウィンドウサイズ変更
		SetWindowSize(width, height);

		// 背景色の設定
		SetBackgroundColor(220, 220, 220);
	}

	void Renderer::Finalize() {

		// Dxライブラリの終了処理
		DxLib_End();
	}

	const void* Renderer::GetBackBuffer() const {
		// D3DImageに描画するために
		// バックバッファへのポインタを取得できるようにする
		return GetUseDirect3D9BackBufferSurface();
	}

	void Renderer::SetAudioData(const shared_ptr<AudioData> audioData) {

		// 参照
		_audioData = audioData;

		// デフォルトの設定で描画データ準備
		SetRenderingData(128, 50, 0, 128);
	}

	void Renderer::SetRenderingData(uint32_t waveWidth, uint32_t waveHeight, uint32_t sampleStart, uint32_t sampleLength) {

		if (!_audioData) return;

		_waveDrawingData.clear();
		auto channlCount = _audioData->GetChannelCount();
		_waveDrawingData.resize(channlCount);

		// サイズを確保
		for (size_t i = 0; i < (size_t)channlCount; i++) {
			_waveDrawingData[i].clear();
			_waveDrawingData[i].resize(waveWidth);
		};

		_waveWidth = waveWidth;
		_waveHeight = waveHeight;
		
		if (waveWidth == 0 || waveHeight == 0) return;

		auto start = sampleStart * channlCount;
		auto length = sampleLength * channlCount;
		auto delta = (float)sampleLength / waveWidth * channlCount;
		auto sampleData = new int[length];
		_audioData->ReadSamples(start, length, &sampleData);

		auto dump = vector<uint32_t>();
		for (size_t i = 0; i < waveWidth; i++) {

			auto base = start + (uint32_t)(delta * i);
			dump.push_back(base);
			for (size_t j = 0; j < (size_t)channlCount; j++) {
				auto sample = sampleData[base + j];

				// 高さを直してコピー
				auto drawData = (float)sample / 128 * waveHeight;
				_waveDrawingData[j][i] = (int)drawData;
			}
		}

		delete[] sampleData;
	}

	void Renderer::ChangeDrawSize(int width, int height) {
		SetWindowSize(width, height);
		_width = width;
		_height = height;
	}

	void Renderer::DrawWave() const {

		//画面を消す
		ClearDrawScreen();

		auto waveColor = GetColor(0, 0, 255);
		auto margin = 20;

		auto y = _waveHeight;
		for (auto&& channel : _waveDrawingData) {
			
			auto prev = channel[0];
			auto x = 0;

			for (size_t i = 1; i < _waveWidth; i++) {
				auto s = channel[i];
				DrawLine(x + i - 1, y + prev, x + i, y + s, waveColor);
				prev = s;
			}

			y += margin + _waveHeight;
		}

		ScreenFlip();
	}
}