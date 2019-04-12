#include "stdafx.h"
#include "../include/SoundEffectTool.h"
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

	void Renderer::ChangeDrawSize(int width, int height) {
		SetWindowSize(width, height);
		_width = width;
		_height = height;
	}

	void Renderer::DrawWave() const {

		//画面を消す
		ClearDrawScreen();

		// デバッグ
		DrawBox(0, 0, 100, 100, GetColor(255, 0, 0), 1);

		ScreenFlip();
	}
}