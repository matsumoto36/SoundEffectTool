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

	void Renderer::CalcRenderingData(PointInt pixelSize, uint32_t sampleStart, uint32_t sampleLength) {

		if (!_audioData) return;
		if (pixelSize.X == 0 || pixelSize.Y == 0) return;

		auto channlCount = _audioData->GetChannelCount();
		auto pointList = vector<vector<vector<int>>>();	// チャンネル・ピクセル位置ごとの点のリスト
		_waveDrawingPoints.clear();
		_waveDrawingPoints.resize(channlCount);
		pointList.resize(channlCount);

		_waveSize = pixelSize;
		_drawSize = PointInt(_waveSize.X, _drawMarginY * 2 + (_waveSize.Y + _waveMarginY) * channlCount - _waveMarginY);

		auto start = sampleStart * channlCount;
		auto length = sampleLength * channlCount;
		auto maxHeight = 0;

		// サンプルデータを取得
		auto sampleData = new int[length];
		_audioData->ReadSamples(start, length, &sampleData);

		// 最大値を求める
		for (size_t i = 0; i < length; i++) {
			auto sample = abs(sampleData[i]);
			if (sample > maxHeight) {
				maxHeight = sample;
			}
		}

		// サイズを確保
		for (size_t i = 0; i < (size_t)channlCount; i++) {
			_waveDrawingPoints[i].clear();
			pointList[i].resize(pixelSize.X);
		};

		auto delta = (float)pixelSize.X / sampleLength;
		// ピクセル位置ごとの高さのリストに変換
		for (size_t i = 0; i < sampleLength; i++) {
			// 横のピクセル位置を計算
			auto x = (size_t)(delta * i);
			for (size_t j = 0; j < (size_t)channlCount; j++) {
				auto sample = sampleData[i * channlCount + j];
				pointList[j][x].push_back(sample);
			}
		}

		delete[] sampleData;

		// 圧縮して追加する
		for (size_t i = 0; i < (size_t)pixelSize.X; i++) {
			for (size_t j = 0; j < (size_t)channlCount; j++) {
				auto dataCount = pointList[j][i].size();
				if (dataCount == 0U) continue;	// ない場合は追加しない
				if (dataCount == 1U) {
					// 高さを直して追加する
					auto drawData = (float)pointList[j][i][0] / maxHeight * (pixelSize.Y / 2.0f);
					_waveDrawingPoints[j].push_back(PointInt(i, (int)drawData));
				}
				else {
					// 最小と最大を取得
					auto sampleMinMax = PointInt(pointList[j][i][0], pointList[j][i][0]);
					for (size_t k = 1; k < dataCount; k++) {
						auto p = pointList[j][i][k];
						if (p < sampleMinMax.X) sampleMinMax.X = p;
						if (p > sampleMinMax.Y) sampleMinMax.Y = p;
					}

					// 高さを直して追加する
					auto drawData = (float)sampleMinMax.X / maxHeight * (pixelSize.Y / 2.0f);
					_waveDrawingPoints[j].push_back(PointInt(i, (int)drawData));
					drawData = (float)sampleMinMax.Y / maxHeight * (pixelSize.Y / 2.0f);
					_waveDrawingPoints[j].push_back(PointInt(i, (int)drawData));
				}
			}

		}
		
	}

	void Renderer::ChangeDrawSize(PointInt size) {
		SetWindowSize(size.X, size.Y);
	}

	void Renderer::DrawWave(float playRatio) const {

		//画面を消す
		ClearDrawScreen();

		const auto waveColor = GetColor(0, 0, 255);
		const auto waveBackGroundColor = GetColor(220, 220, 220);

		const auto zeroLineColor = GetColor(32, 32, 32);
		const auto playRatioColor = GetColor(4, 4, 4);

		auto waveSizeYHalf = _waveSize.Y / 2;
		auto offsetX = 0;
		auto offsetY = _drawMarginY + waveSizeYHalf;
		for (auto&& channel : _waveDrawingPoints) {
			
			// 波形の中心を引く
			DrawLine(0, offsetY, _waveSize.X, offsetY, zeroLineColor);

			// 波形の背景を描く
			DrawFillBox(0, offsetY - waveSizeYHalf, _waveSize.X, offsetY + waveSizeYHalf, waveBackGroundColor);

			// 点のリストを描画する
			auto prevSample = channel[0];
			for (size_t i = 1; i < channel.size(); i++) {
				auto currentSample = channel[i];
				DrawLine(offsetX + prevSample.X, offsetY + prevSample.Y, offsetX + currentSample.X, offsetY + currentSample.Y, waveColor);
				prevSample = currentSample;
			}

			offsetY += _waveSize.Y + _waveMarginY;
		}

		// 再生位置に縦線を引く
		auto positionX = int(_waveSize.X * playRatio + offsetX);
		DrawLine(positionX, 0, positionX, _drawSize.Y, playRatioColor);


		ScreenFlip();
	}
}