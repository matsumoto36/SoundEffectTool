#include "stdafx.h"
#include "../include/Renderer.h"
#include "DxLib.h"

using namespace DxLib;

namespace SoundEffectTool {

	Renderer::Renderer() = default;
	Renderer::~Renderer() = default;

	void Renderer::Initialize(HWND hwnd, PointInt size) {

		// Dxライブラリ初期化設定
		SetUseDirect3DVersion(DX_DIRECT3D_9EX);				// DirectX 9EXにする
		SetUserWindow(hwnd);								// 描画ウィンドウの設定
		ChangeWindowMode(TRUE);								// ウィンドウモードに設定
		SetWindowSizeChangeEnableFlag(TRUE);				// ウィンドウサイズを可変に

		GetDefaultState(&_maxSize.X, &_maxSize.Y, NULL);	// デスクトップのウィンドウサイズを取得
		SetGraphMode(_maxSize.X, _maxSize.Y, 32);			// グラフィックモードの設定(最大)

		SetAlwaysRunFlag(TRUE);								// 非アクティブ時も処理続行
		SetDrawScreen(DX_SCREEN_BACK);						// 描画先をバックバッファへ設定
		SetUseFPUPreserveFlag(TRUE);						// FPUの精度を落とさない
		SetWaitVSyncFlag(FALSE);							// VSync同期を無効
		SetOutApplicationLogValidFlag(FALSE);				// ログ出力停止
		SetDoubleStartValidFlag(TRUE);						// 多重起動を許可
		SetUseIMEFlag(TRUE);								// IMEを有効
			
		SetDrawValidGraphCreateFlag(TRUE);					// 描画先に設定できる画像を作成できる

		// Dxライブラリの初期化
		if (DxLib_Init() == -1) return;

		// ウィンドウサイズ変更
		SetWindowSize(size.X, size.Y);

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

	void Renderer::CalcRenderingData(PointInt pixelSize, uint32_t sampleStart, uint32_t sampleLength, uint32_t toSamplingRate) {

		if (!_audioData) return;
		if (pixelSize.X == 0 || pixelSize.Y == 0) return;

		auto channlCount = _audioData->GetChannelCount();
		auto wavePoints = vector<vector<PointInt>>(channlCount);

		// 描画のサイズを計算(WPFのImageのサイズに使用される)
		_drawSize = PointInt(pixelSize.X, _drawMarginY * 2 + (pixelSize.Y + _waveMarginY) * channlCount - _waveMarginY);

		auto start = sampleStart * channlCount;
		auto length = sampleLength * channlCount;
		auto maxHeight = 0;

		// サンプルデータを取得
		auto sampleData = new int[length];
		_audioData->ReadSamples(start, length, &sampleData);

		// 最大値を求める
		for (size_t i = 0; i < length; i++) {
			auto sample = sampleData[i];
			if (abs(sample) > maxHeight) {
				maxHeight = sample;
			}
		}

		// データを間引きして追加 44100 => toSamplingRate
		auto samplesPerSec = _audioData->GetFormat().nSamplesPerSec;
		auto sampleMag = float(samplesPerSec / toSamplingRate);
		auto dataCount = size_t(sampleLength / sampleMag);

		// サイズを確保
		for (size_t i = 0; i < (size_t)channlCount; i++) {
			wavePoints[i].resize(dataCount);
		};

		auto deltaI = (float)1 / dataCount;
		for (size_t i = 0; i < dataCount; i++) {

			auto base = size_t(i * sampleMag);
			for (size_t j = 0; j < (size_t)channlCount; j++) {
				auto sample = sampleData[base * channlCount + j];

				// 高さを直して追加する
				auto drawData = (float)sample / maxHeight * (pixelSize.Y / 2.0f);
				wavePoints[j][i] = PointInt(int(pixelSize.X * i * deltaI), (int)drawData);
			}
		}

		delete[] sampleData;

		// あらかじめ描画する
		if (_waveRenderHandler != -1) DeleteGraph(_waveRenderHandler);
		_waveRenderHandler = MakeGraph(_drawSize.X, _drawSize.Y);
		DrawWave(wavePoints, pixelSize);
	}

	void Renderer::ChangeDrawSize(PointInt size) {
		SetWindowSize(size.X, size.Y);
	}

	void Renderer::DrawWave(vector<vector<PointInt>> wavePoints, PointInt pixelSize) const {

		// ターゲットを指定
		SetDrawScreen(_waveRenderHandler);

		//画面を消す
		ClearDrawScreen();

		const auto waveColor = GetColor(0, 0, 255);
		const auto waveBackGroundColor = GetColor(220, 220, 220);

		const auto zeroLineColor = GetColor(32, 32, 32);

		auto waveSizeYHalf = pixelSize.Y / 2;
		auto offsetX = 0;
		auto offsetY = _drawMarginY + waveSizeYHalf;
		for (auto&& channel : wavePoints) {
			
			// 波形の中心を引く
			DrawLine(0, offsetY, pixelSize.X, offsetY, zeroLineColor);

			// 波形の背景を描く
			DrawFillBox(0, offsetY - waveSizeYHalf, pixelSize.X, offsetY + waveSizeYHalf, waveBackGroundColor);

			// 点のリストを描画する
			auto prevSample = channel[0];
			for (size_t i = 1; i < channel.size(); i++) {
				auto currentSample = channel[i];
				DrawLine(offsetX + prevSample.X, offsetY + prevSample.Y, offsetX + currentSample.X, offsetY + currentSample.Y, waveColor);
				prevSample = currentSample;
			}

			offsetY += pixelSize.Y + _waveMarginY;
		}
	}

	void Renderer::Draw(PointInt waveOffset, float playRatio) const {

		// ターゲットを指定
		SetDrawScreen(DX_SCREEN_BACK);

		//画面を消す
		ClearDrawScreen();

		// 波形を描画
		DrawGraph(waveOffset.X, waveOffset.Y, _waveRenderHandler, FALSE);
		
		const auto playRatioColor = GetColor(4, 4, 4);

		// 再生位置に縦線を引く
		auto positionX = int(_drawSize.X * playRatio + waveOffset.X);
		DrawLine(positionX, 0, positionX, _drawSize.Y, playRatioColor);

		ScreenFlip();
	}

}