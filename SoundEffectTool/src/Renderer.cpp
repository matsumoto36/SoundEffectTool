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
		SetBackgroundColor(32, 32, 32);
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
		//const unique_ptr<uint8_t[]>& waveData, UINT32 length, int channels
		// 参照
		_audioData = audioData;

		// デフォルトの設定
		//SetRenderingData(1024, 100, 0, length / channels);
		SetRenderingData(1024, 100, 0, 2048);
	}

	void Renderer::SetRenderingData(int waveWidth, int waveHeight, UINT32 start, UINT32 end) {

		_waveDrawingData.clear();
		_waveDrawingData.resize(_wavePerChannel.size());
		_waveWidth = waveWidth;
		_waveHeight = waveHeight;
		_waveLength = end - start;


		_wavePerChannel.clear();
		_wavePerChannel.resize(channels);

		_waveLength = length;

		// チャンネルごとの位置格納用
		int* currents = new int[channels];

		for (size_t i = 0; i < (size_t)channels; i++) {
			// 割り切れるはず
			_wavePerChannel[i] = make_unique<uint8_t[]>(length / channels);
			currents[i] = 0;
		}

		UINT32 position = 0;
		auto finished = false;
		_waveMax = 255;

		// 波形をチャンネルごとに分けてコピー
		while (!finished) {
			for (size_t i = 0; i < _wavePerChannel.size(); i++) {

				auto wave = waveData[position];
				//if (wave > _waveMax) _waveMax = wave;

				// 比率でコピー
				_wavePerChannel[i][currents[i]++] = wave;

				if (length < ++position) {
					finished = true;
					break;
				}
			}
		}

		delete[] currents;

		
		for (size_t i = 0; i < _waveDrawingData.size(); i++) {

			//_waveDrawingData[i] = make_unique<uint8_t[]>(waveWidth);
			_waveDrawingData[i].resize(waveWidth);

			for (size_t j = 0; j < waveWidth; j++) {
				size_t position = start + (float)j / waveWidth * _waveLength;
				auto sample = (float)_wavePerChannel[i][position] / _waveMax * waveHeight;
				_waveDrawingData[i][j] = sample;
			}
		}
	}

	void Renderer::ChangeDrawSize(int width, int height) {
		SetWindowSize(width, height);
		_width = width;
		_height = height;
	}

	void Renderer::DrawWave() const {

		//画面を消す
		ClearDrawScreen();

		if (_waveLength <= 0) return;

		auto waveColor = GetColor(0, 0, 255);
		auto margin = 20;

		auto y = 0;
		for (auto&& channel : _waveDrawingData) {
			
			auto prev = channel[0] - 128;
			auto x = 0;

			for (size_t i = 1; i < _waveWidth; i++) {
				auto s = channel[i] - 128;
				DrawLine(x + i - 1, y + prev, x + i, y + s, waveColor);
				prev = s;
			}

			y += margin + _waveHeight;
		}

		ScreenFlip();
	}
}