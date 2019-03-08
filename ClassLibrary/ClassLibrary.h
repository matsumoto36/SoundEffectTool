#pragma once

using namespace System;

namespace SoundEffectTool {

	public ref class SoundEffectToolHelper {

	public :
		
		static bool Initialize();
		static void Final();

		// ウィンドウの生成
		static const IntPtr CreateDxView(IntPtr, IntPtr%, int, int);

		// 描画サイズを変更する
		static void ChangeDrawSize(IntPtr, int, int);
	};
}
